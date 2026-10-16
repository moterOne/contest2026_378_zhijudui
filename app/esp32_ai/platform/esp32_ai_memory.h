/****************************************************************************
 * Contest 2026 team 378 esp32_ai - platform/esp32_ai_memory.h
 *
 * Directed SRAM/PSRAM allocation for the Barista port.
 *
 * The original Arduino sketch used esp-idf heap_caps_malloc(MALLOC_CAP_*).
 * NuttX exposes no such capability API from applications, so this layer
 * pins the two tiers to the NuttX heaps that the AI board configuration
 * establishes:
 *
 *   PSRAM tier  -> kumm_* (user heap).  With
 *                 CONFIG_ESPRESSIF_SPIRAM_USER_HEAP + CONFIG_MM_KERNEL_HEAP
 *                 the user heap IS the PSRAM region (see
 *                 arch/risc-v/src/common/espressif/esp_allocateheap.c).
 *   SRAM tier   -> kmm_* (kernel heap, internal RAM), only in flat builds
 *                 with CONFIG_MM_KERNEL_HEAP where kmm_* is callable
 *                 directly.
 *
 * If the kernel heap is unavailable (protected/kernel build), the SRAM tier
 * falls back to the user heap and every fallback is counted and reported so
 * that "measured on PSRAM/SRAM" labels are never silently wrong.
 *
 * All functions in this header are app-facing wrappers; they never free
 * memory they did not allocate.
 ****************************************************************************/

#ifndef ESP32_AI_MEMORY_H
#define ESP32_AI_MEMORY_H

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C"
{
#endif

/* Memory tiers the model working set is split across. */
#define ESP32_AI_MEM_SRAM  0   /* hot activations, norms, scratch */
#define ESP32_AI_MEM_PSRAM 1   /* model image, staged int8 weights, KV cache */

/****************************************************************************
 * Name: esp32_ai_mem_alloc
 *
 * Description:
 *   Allocate n bytes from the requested tier.  Returns NULL on failure.
 *   Alignment is 8 bytes for PSRAM and native for SRAM (memalign(8, n)).
 *
 ****************************************************************************/
void *esp32_ai_mem_alloc(int tier, size_t n);

/****************************************************************************
 * Name: esp32_ai_mem_free
 *
 * Description:
 *   Free a pointer returned by esp32_ai_mem_alloc.  NULL is accepted.
 *
 ****************************************************************************/
void esp32_ai_mem_free(void *p);

/****************************************************************************
 * Name: esp32_ai_mem_tier_free
 *
 * Description:
 *   Report the free bytes of the given tier (heap-internal accounting).
 *
 ****************************************************************************/
size_t esp32_ai_mem_tier_free(int tier);

/****************************************************************************
 * Name: esp32_ai_mem_psram_present
 *
 * Description:
 *   Best-effort runtime check that the user heap is really PSRAM-backed:
 *   true when CONFIG_ESPRESSIF_SPIRAM_USER_HEAP is set AND the user heap
 *   arena size is at least CONFIG_ESP32_AI_EXPECT_PSRAM_BYTES.
 *
 * Returned Value:
 *   1 if PSRAM heap is present, 0 otherwise.
 *
 ****************************************************************************/
int esp32_ai_mem_psram_present(void);

/****************************************************************************
 * Name: esp32_ai_mem_sram_fallbacks
 *
 * Description:
 *   Number of SRAM-tier allocations that fell back to the user heap
 *   (because the kernel heap is not available in this build).
 *
 ****************************************************************************/
unsigned long esp32_ai_mem_sram_fallbacks(void);

#ifdef __cplusplus
}
#endif

#endif /* ESP32_AI_MEMORY_H */
