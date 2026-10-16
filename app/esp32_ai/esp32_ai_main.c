/****************************************************************************
 * Contest 2026 team 378 esp32_ai - esp32_ai_main.c
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Port of firmware/esp32_barista/esp32_barista.ino to a NuttX/OpenVela NSH
 * application, per docs/esp32-ai-移植至ESP32-P4X-OpenVela部署方案.md.
 *
 * Deliberate differences from the Arduino sketch:
 *   - Runs as a single NSH command on one core; no FreeRTOS tasks, no
 *     display (USE_DISPLAY=0 equivalent), no USB-CDC Serial specifics.
 *   - Model access goes through the bounded storage-view layer
 *     (platform/esp32_ai_storage.[ch]) instead of esp_partition_mmap.
 *   - Directed allocation goes through platform/esp32_ai_memory.[ch]
 *     instead of heap_caps_*.
 *   - The model is loaded into RAM (PSRAM in the AI config) because no
 *     mmap of the high flash region is assumed on this target.
 *   - Every entry point can exit back to NSH; chat ends on EOF or "quit".
 *
 * Subcommands:
 *   esp32_ai            - print usage + model/resource summary
 *   esp32_ai info       - model/tokenizer/memory summary
 *   esp32_ai selftest   - P1 self-check (storage, header, random reads,
 *                          alloc/free, memory tiers)
 *   esp32_ai chat       - P2 serial Q&A MVP (requires generated resources)
 ****************************************************************************/

#include <nuttx/config.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <ctype.h>

#include "esp32_ai_memory.h"
#include "esp32_ai_storage.h"
#include "esp32_ai_time.h"
#include "esp32_ai_hash.h"

#include "llm.h"
#include "bpe_tokenizer.h"

#ifdef CONFIG_ESP32_AI_BARISTA_RESOURCES
#  include "generated/tokenizer_encoder.h"
#  include "generated/barista_words.h"
#  include "generated/barista_out2in.h"
#endif

/* Version of this build of the port; shown on the splash line. */
#define ESP32_AI_PORT_VERSION "0.1"

/* P1 self-check probe: number of random reads across the region. */
#define SELFTEST_RANDOM_READS 8
#define SELFTEST_PROBE_BYTES  16

/* P2 chat limits, mirroring the original sketch. */
#define ESP32_AI_ANSWER_ROOM   56
#define ESP32_AI_MAX_PIECES    48

static Model model;
static Scratch scratch;
static BpeTokenizer tokenizer;
static int ready = 0;

/* ---- P1 self-check ------------------------------------------------------ */

static void selftest_header(const uint8_t *hdr, char *out, size_t outlen)
{
  uint32_t magic;
  uint32_t version;
  uint32_t header_bytes;
  uint32_t flags;
  uint32_t in_vocab;
  uint32_t out_vocab;

  memcpy(&magic, hdr, 4);
  memcpy(&version, hdr + 4, 4);
  memcpy(&header_bytes, hdr + 8, 4);
  memcpy(&flags, hdr + 12, 4);
  memcpy(&in_vocab, hdr + 16, 4);
  memcpy(&out_vocab, hdr + 20, 4);

  snprintf(out, outlen,
           "magic=%08x ver=%u hdr=%u flags=%u vin=%u vout=%u",
           (unsigned)magic, (unsigned)version, (unsigned)header_bytes,
           (unsigned)flags, (unsigned)in_vocab, (unsigned)out_vocab);
}

static int selftest_run(void)
{
  esp32_ai_storage_t s;
  uint8_t hdr[LLM_HEADER_BYTES];
  char desc[160];
  ssize_t n;
  int i;
  int rc = 0;

  printf("esp32_ai selftest (P1) - port %s\n", ESP32_AI_PORT_VERSION);

  /* 1. Storage view. */
  if (esp32_ai_storage_open(&s) != 0)
    {
      printf("FAIL storage open /dev/model: %s\n", strerror(errno));
      printf("  is CONFIG_ESP32_AI_MODEL_STORAGE=y and /dev/model registered?\n");
      return 1;
    }

  printf("OK   storage: %s size=0x%x (%u B)\n",
         s.devpath, (unsigned)s.size, (unsigned)s.size);

  /* 2. Read the model header and validate it. */
  n = esp32_ai_storage_read(&s, hdr, sizeof(hdr), 0);
  if (n != (ssize_t)sizeof(hdr))
    {
      printf("FAIL header read: %zd bytes (%s)\n", n, strerror(errno));
      rc = 1;
      goto out_close;
    }

  selftest_header(hdr, desc, sizeof(desc));
  printf("OK   header: %s\n", desc);

  {
    uint32_t magic;
    memcpy(&magic, hdr, 4);
    if (magic != LLM_MAGIC)
      {
        printf("FAIL magic 0x%08x != LLM_MAGIC 0x%08x (no model written?)\n",
               (unsigned)magic, (unsigned)LLM_MAGIC);
        rc = 1;
        goto out_close;
      }
  }

  /* 3. Size contract: expected model size must fit the region. */
  printf("INFO model size: expected=0x%x (%u B), region=%u B\n",
         (unsigned)CONFIG_ESP32_AI_MODEL_SIZE,
         (unsigned)CONFIG_ESP32_AI_MODEL_SIZE, (unsigned)s.size);
  if ((uint32_t)CONFIG_ESP32_AI_MODEL_SIZE > s.size)
    {
      printf("FAIL model size exceeds storage region\n");
      rc = 1;
      goto out_close;
    }

  /* 4. Limited random reads (P1: 限量随机读). */
  for (i = 0; i < SELFTEST_RANDOM_READS; i++)
    {
      uint32_t off = (uint32_t)(((uint64_t)i * s.size) /
                                SELFTEST_RANDOM_READS);
      uint8_t probe[SELFTEST_PROBE_BYTES];

      if (off + SELFTEST_PROBE_BYTES > s.size)
        {
          off = s.size - SELFTEST_PROBE_BYTES;
        }

      n = esp32_ai_storage_read(&s, probe, sizeof(probe), off);
      if (n != (ssize_t)sizeof(probe))
        {
          printf("FAIL random read @0x%x: %zd (%s)\n",
                 (unsigned)off, n, strerror(errno));
          rc = 1;
          goto out_close;
        }
    }

  printf("OK   %d bounded random reads\n", SELFTEST_RANDOM_READS);

  /* 5. Bounds check must reject an out-of-region read. */
  {
    uint8_t probe[8];
    errno = 0;
    n = esp32_ai_storage_read(&s, probe, sizeof(probe), s.size - 3);
    if (n >= 0)
      {
        printf("FAIL out-of-region read was not rejected\n");
        rc = 1;
        goto out_close;
      }

    printf("OK   out-of-region read rejected (errno=%s)\n", strerror(errno));
  }

  /* 6. Alloc/free round trip on both tiers. */
  {
    enum { TRIAL = 4096 };
    void *p;

    p = esp32_ai_mem_alloc(ESP32_AI_MEM_SRAM, TRIAL);
    if (p == NULL)
      {
        printf("FAIL sram alloc %d B\n", TRIAL);
        rc = 1;
        goto out_close;
      }

    memset(p, 0xA5, TRIAL);
    esp32_ai_mem_free(p);

    p = esp32_ai_mem_alloc(ESP32_AI_MEM_PSRAM, TRIAL);
    if (p == NULL)
      {
        printf("FAIL psram alloc %d B\n", TRIAL);
        rc = 1;
        goto out_close;
      }

    memset(p, 0x5A, TRIAL);
    esp32_ai_mem_free(p);
    printf("OK   alloc/free round trip (4 KiB SRAM + 4 KiB PSRAM)\n");
  }

  /* 7. Whole-image sizing: can the region be loaded at all? */
  {
    void *img = esp32_ai_mem_alloc(ESP32_AI_MEM_PSRAM, s.size);
    if (img == NULL)
      {
        printf("FAIL cannot allocate %u B for the model image "
               "(PSRAM heap missing or too small)\n", (unsigned)s.size);
        rc = 1;
        goto out_close;
      }

    n = esp32_ai_storage_load_image(&s, img, s.size);
    if (n != (ssize_t)s.size)
      {
        printf("FAIL load image: %zd (%s)\n", n, strerror(errno));
        esp32_ai_mem_free(img);
        rc = 1;
        goto out_close;
      }

    {
      /* FNV-1a over the loaded image, mirroring the original fp print.
       * This is NOT the SHA-256 recorded for provenance.
       */
      uint32_t fp = esp32_ai_fnv1a((const uint8_t *)img, (size_t)s.size);
      printf("OK   loaded %u B, FNV-1a(fp)=%08x (device-side only)\n",
             (unsigned)n, (unsigned)fp);
    }

    esp32_ai_mem_free(img);
  }

  /* 8. Memory tier report. */
  printf("INFO memory: SRAM free=%u B, PSRAM/user free=%u B, "
         "psram_present=%d, sram_fallbacks=%lu\n",
         (unsigned)esp32_ai_mem_tier_free(ESP32_AI_MEM_SRAM),
         (unsigned)esp32_ai_mem_tier_free(ESP32_AI_MEM_PSRAM),
         esp32_ai_mem_psram_present(),
         esp32_ai_mem_sram_fallbacks());

  printf("PASS selftest\n");

out_close:
  esp32_ai_storage_close(&s);
  return rc;
}

/* ---- tokenizer / model resource helpers --------------------------------- */

#ifdef CONFIG_ESP32_AI_BARISTA_RESOURCES

static int tokenizer_load(void)
{
  if (bpe_tokenizer_load(TOKENIZER_ENCODER_ASSET,
                         TOKENIZER_ENCODER_ASSET_SIZE, &tokenizer) != 0)
    {
      printf("tokenizer load failed\n");
      return -1;
    }

  return 0;
}

/* Model-vs-tables consistency checks, mirroring the original setup(). */
static int model_tables_consistent(void)
{
  Cfg *c = &model.c;

  if (model.out_vocab != BARISTA_WORD_COUNT)
    {
      printf("word table mismatch: model %d vs table %d\n",
             model.out_vocab, BARISTA_WORD_COUNT);
      return 0;
    }

  if ((int)tokenizer.active_vocab > c->vocab)
    {
      printf("tokenizer/model mismatch: encoder can emit %u ids, "
             "model reads %d\n",
             (unsigned)tokenizer.active_vocab, c->vocab);
      return 0;
    }

  {
    uint16_t max_in = 0;
    int k;

    for (k = 0; k < BARISTA_WORD_COUNT; k++)
      {
        if (BARISTA_OUT2IN[k] > max_in)
          {
            max_in = BARISTA_OUT2IN[k];
          }
      }

    if ((int)max_in + 1 != c->vocab)
      {
        printf("out2in/model mismatch: mapped ids reach %u, "
               "model reads %d\n",
               (unsigned)max_in, c->vocab);
        return 0;
      }
  }

  return 1;
}

#endif /* CONFIG_ESP32_AI_BARISTA_RESOURCES */

/* ---- P2 Q&A -------------------------------------------------------------- */

#ifdef CONFIG_ESP32_AI_BARISTA_RESOURCES

static void answer(const char *question)
{
  uint16_t ids[BTK_MAX_INPUT_BYTES];
  int n;
  int pos;
  int step;
  int pieces_out;
  uint64_t t0;

  n = bpe_encode_ascii(&tokenizer, question, ids,
                       (int)(sizeof(ids) / sizeof(ids[0])));
  if (n == BTK_ERR_NOT_ASCII)
    {
      printf("(ascii only)\n");
      return;
    }

  if (n <= 0 || n > model.c.seq_len - ESP32_AI_ANSWER_ROOM)
    {
      printf("(question too long)\n");
      return;
    }

  t0 = esp32_ai_time_us();
  pos = 0;
  for (int i = 0; i < n; i++)
    {
      llm_forward(&model, ids[i], pos++, &scratch);
    }

  llm_forward(&model, BARISTA_OUT2IN[BARISTA_BOS], pos++, &scratch);

  printf("A: ");
  pieces_out = 0;
  for (step = 0; step < ESP32_AI_MAX_PIECES && pos < model.c.seq_len; step++)
    {
      int best = 0;
      int k;

      for (k = 1; k < BARISTA_WORD_COUNT; k++)
        {
          if (scratch.logits[k] > scratch.logits[best])
            {
              best = k;
            }
        }

      if (best == BARISTA_EOS)
        {
          break;
        }

      {
        const char *w = BARISTA_WORDS[best];
        int punct = (w[1] == '\0' && strchr(".,:;?", w[0]) != NULL);

        if (pieces_out && !punct)
          {
            printf(" ");
          }

        printf("%s", w);
        fflush(stdout);
      }

      pieces_out++;
      llm_forward(&model, BARISTA_OUT2IN[best], pos++, &scratch);
    }

  {
    double ms = (double)(esp32_ai_time_us() - t0) / 1000.0;
    printf("\n[%d pieces, %.0f ms, %.1f pieces/s]\n",
           pieces_out, ms, pieces_out * 1000.0 / (ms > 0.0 ? ms : 1.0));
  }
}

/* malloc-style callback used by llm_stage_core_int8_alloc: everything the
 * staging walker allocates lands on the PSRAM tier.
 */
static void *chat_stage_alloc(size_t n)
{
  return esp32_ai_mem_alloc(ESP32_AI_MEM_PSRAM, n);
}

/* One-shot setup: load tokenizer, load+bind model, alloc scratch, stage
 * int8 weights.  Mirrors the original setup() minus display/dual-core.
 */
static int chat_setup(void)
{
  esp32_ai_storage_t s;
  void *img;
  ssize_t n;
  int staged;

  if (tokenizer_load() != 0)
    {
      return -1;
    }

  if (esp32_ai_storage_open(&s) != 0)
    {
      printf("model storage open failed: %s\n", strerror(errno));
      return -1;
    }

  img = esp32_ai_mem_alloc(ESP32_AI_MEM_PSRAM, s.size);
  if (img == NULL)
    {
      printf("model image alloc failed (%u B): PSRAM heap not usable\n",
             (unsigned)s.size);
      esp32_ai_storage_close(&s);
      return -1;
    }

  n = esp32_ai_storage_load_image(&s, img, s.size);
  if (n != (ssize_t)s.size)
    {
      printf("model image load failed: %zd (%s)\n", n, strerror(errno));
      esp32_ai_mem_free(img);
      esp32_ai_storage_close(&s);
      return -1;
    }

  if (llm_load((const uint8_t *)img, &model) != 0)
    {
      printf("bad model (llm_load rejected the image)\n");
      esp32_ai_mem_free(img);
      esp32_ai_storage_close(&s);
      return -1;
    }

  {
    Cfg *c = &model.c;
    printf("model: Vin=%d Vout=%d D=%d L=%d H=%d F=%d P=%d\n",
           c->vocab, model.out_vocab, c->dim, c->n_layers, c->n_heads,
           c->ffn, c->ple_dim);
  }

  if (!model_tables_consistent())
    {
      esp32_ai_mem_free(img);
      esp32_ai_storage_close(&s);
      return -1;
    }

  if ((uint32_t)model.image_bytes > (uint32_t)CONFIG_ESP32_AI_MODEL_SIZE)
    {
      printf("model image_bytes=%u exceeds expected size 0x%x; "
             "stale or wrong model release?\n",
             (unsigned)model.image_bytes,
             (unsigned)CONFIG_ESP32_AI_MODEL_SIZE);
      esp32_ai_mem_free(img);
      esp32_ai_storage_close(&s);
      return -1;
    }

  /* Scratch layout follows the original three-tier split as far as this
   * build allows: hot working set on the SRAM tier, KV cache on PSRAM.
   */
  {
    Cfg *c = &model.c;
    int D = c->dim;
    int L = c->n_layers;
    int P = c->ple_dim;
    int F = c->ffn;
    int S = c->seq_len;

    scratch.x      = esp32_ai_mem_alloc(ESP32_AI_MEM_SRAM, (size_t)D * 4);
    scratch.h      = esp32_ai_mem_alloc(ESP32_AI_MEM_SRAM,
                                       (size_t)(F > D ? F : D) * 4);
    scratch.qkv    = esp32_ai_mem_alloc(ESP32_AI_MEM_SRAM, (size_t)3 * D * 4);
    scratch.att    = esp32_ai_mem_alloc(ESP32_AI_MEM_SRAM, (size_t)D * 4);
    scratch.g1     = esp32_ai_mem_alloc(ESP32_AI_MEM_SRAM, (size_t)F * 4);
    scratch.g2     = esp32_ai_mem_alloc(ESP32_AI_MEM_SRAM,
                                       (size_t)(P > F ? P : F) * 4);
    scratch.ple    = esp32_ai_mem_alloc(ESP32_AI_MEM_SRAM, (size_t)L * P * 4);
    scratch.tmpP   = esp32_ai_mem_alloc(ESP32_AI_MEM_SRAM, (size_t)L * P * 4);
    scratch.trow   = esp32_ai_mem_alloc(ESP32_AI_MEM_SRAM, (size_t)L * P * 4);
    scratch.logits = esp32_ai_mem_alloc(ESP32_AI_MEM_SRAM,
                                        (size_t)model.out_vocab * 4);
    scratch.scores = esp32_ai_mem_alloc(ESP32_AI_MEM_SRAM, (size_t)S * 4);
    scratch.kcache = esp32_ai_mem_alloc(ESP32_AI_MEM_PSRAM,
                                        (size_t)L * S * D * 4);
    scratch.vcache = esp32_ai_mem_alloc(ESP32_AI_MEM_PSRAM,
                                        (size_t)L * S * D * 4);

    if (scratch.x == NULL || scratch.h == NULL || scratch.qkv == NULL ||
        scratch.att == NULL || scratch.g1 == NULL || scratch.g2 == NULL ||
        scratch.ple == NULL || scratch.tmpP == NULL || scratch.trow == NULL ||
        scratch.logits == NULL || scratch.scores == NULL ||
        scratch.kcache == NULL || scratch.vcache == NULL)
      {
        printf("scratch alloc failed (SRAM tier exhausted?)\n");
        esp32_ai_mem_free(img);
        esp32_ai_storage_close(&s);
        return -1;
      }
  }

  /* Stage the per-position core to int8 on the PSRAM tier, exactly as the
   * original (llm_stage_core_int8_alloc walks core tensors, one allocation
   * each; failures keep the slow int4 path and are counted).
   */
  staged = llm_stage_core_int8_alloc(&model, chat_stage_alloc);

  /* The output head is not part of the core walk; stage it separately. */
  {
    void *b = esp32_ai_mem_alloc(ESP32_AI_MEM_PSRAM,
                                 llm_stage_int8_bytes(&model.out_head));
    if (b != NULL)
      {
        llm_stage_int8(&model.out_head, b);
        staged++;
      }
  }

  printf("int8-staged %d tensors | sram free=%u B | "
         "psram/user free=%u B\n",
         staged,
         (unsigned)esp32_ai_mem_tier_free(ESP32_AI_MEM_SRAM),
         (unsigned)esp32_ai_mem_tier_free(ESP32_AI_MEM_PSRAM));

  /* Provenance line, mirroring the original "build:" print. */
  {
    const uint8_t *ib = (const uint8_t *)img;
    uint32_t fp = esp32_ai_fnv1a(ib, (size_t)model.image_bytes);
    printf("build: magic=%08x bytes=%u fp=%08x sram_fallbacks=%lu\n",
           (unsigned)LLM_MAGIC, (unsigned)model.image_bytes,
           (unsigned)fp, esp32_ai_mem_sram_fallbacks());
  }

  ready = 1;
  return 0;
}

static int chat_run(void)
{
  char line[BTK_MAX_INPUT_BYTES];

  if (chat_setup() != 0)
    {
      return 1;
    }

  printf("READY>\n");
  fflush(stdout);

  while (fgets(line, sizeof(line), stdin) != NULL)
    {
      size_t len = strlen(line);

      while (len > 0 && (line[len - 1] == '\n' || line[len - 1] == '\r'))
        {
          line[--len] = '\0';
        }

      if (len == 0)
        {
          printf("READY>\n");
          fflush(stdout);
          continue;
        }

      if (strcmp(line, "quit") == 0 || strcmp(line, "exit") == 0)
        {
          printf("bye\n");
          break;
        }

      if (len >= sizeof(line) - 1)
        {
          printf("(question too long)\n");
        }
      else
        {
          answer(line);
        }

      printf("READY>\n");
      fflush(stdout);
    }

  /* Free the big allocations so a second invocation can re-run cleanly. */
  free(scratch.kcache);
  free(scratch.vcache);
  memset(&scratch, 0, sizeof(scratch));
  ready = 0;
  return 0;
}

#endif /* CONFIG_ESP32_AI_BARISTA_RESOURCES */

/* ---- info ---------------------------------------------------------------- */

static void info_run(void)
{
  esp32_ai_storage_t s;
  uint8_t hdr[LLM_HEADER_BYTES];
  char desc[160];
  ssize_t n;
  int have_resources = 0;

  printf("esp32_ai port %s - OpenVela/NuttX Barista\n", ESP32_AI_PORT_VERSION);
  printf("  storage region: offset=0x%x size=0x%x (%u B)\n",
         (unsigned)CONFIG_ESP32_AI_MODEL_FLASH_OFFSET,
         (unsigned)CONFIG_ESP32_AI_MODEL_FLASH_SIZE,
         (unsigned)CONFIG_ESP32_AI_MODEL_FLASH_SIZE);
  printf("  expected model: size=0x%x (%u B) sha256=%s\n",
         (unsigned)CONFIG_ESP32_AI_MODEL_SIZE,
         (unsigned)CONFIG_ESP32_AI_MODEL_SIZE,
         CONFIG_ESP32_AI_MODEL_SHA256[0] ? CONFIG_ESP32_AI_MODEL_SHA256
                                         : "(not recorded)");
  printf("  expected PSRAM: %d B\n", CONFIG_ESP32_AI_EXPECT_PSRAM_BYTES);
  printf("  resources: %s\n",
#ifdef CONFIG_ESP32_AI_BARISTA_RESOURCES
         "generated headers present"
#else
         "self-check only (generate resource headers for chat)"
#endif
        );

  if (esp32_ai_storage_open(&s) == 0)
    {
      n = esp32_ai_storage_read(&s, hdr, sizeof(hdr), 0);
      if (n == (ssize_t)sizeof(hdr))
        {
          selftest_header(hdr, desc, sizeof(desc));
          printf("  on-flash header: %s\n", desc);
        }
      else
        {
          printf("  on-flash header: unreadable (%s)\n", strerror(errno));
        }

      esp32_ai_storage_close(&s);
    }
  else
    {
      printf("  on-flash header: /dev/model not available (%s)\n",
             strerror(errno));
    }

  printf("  memory: SRAM free=%u B, PSRAM/user free=%u B, psram_present=%d\n",
         (unsigned)esp32_ai_mem_tier_free(ESP32_AI_MEM_SRAM),
         (unsigned)esp32_ai_mem_tier_free(ESP32_AI_MEM_PSRAM),
         esp32_ai_mem_psram_present());

  printf("  usage: esp32_ai info | selftest | chat\n");
}

/* ---- entry --------------------------------------------------------------- */

int main(int argc, char *argv[])
{
  const char *cmd;

  if (argc < 2)
    {
      info_run();
      return 0;
    }

  cmd = argv[1];

  if (strcmp(cmd, "info") == 0)
    {
      info_run();
      return 0;
    }

  if (strcmp(cmd, "selftest") == 0)
    {
      return selftest_run();
    }

  if (strcmp(cmd, "chat") == 0)
    {
#ifdef CONFIG_ESP32_AI_BARISTA_RESOURCES
      return chat_run();
#else
      printf("esp32_ai chat requires CONFIG_ESP32_AI_BARISTA_RESOURCES and "
             "the generated headers; refusing to answer with a "
             "self-check-only build.\n");
      return 1;
#endif
    }

  printf("usage: esp32_ai [info|selftest|chat]\n");
  return 1;
}
