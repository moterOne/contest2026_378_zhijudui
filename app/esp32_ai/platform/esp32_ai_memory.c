/****************************************************************************
 * Contest 2026 team 378 esp32_ai - platform/esp32_ai_memory.c
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Directed SRAM/PSRAM allocation wrappers (see esp32_ai_memory.h for the
 * tier mapping and its caveats).
 ****************************************************************************/

#include <nuttx/config.h>

#include <stdlib.h>
#include <string.h>
#include <malloc.h>

#ifdef CONFIG_MM_KERNEL_HEAP
#  include <nuttx/kmalloc.h>
#  include <nuttx/mm/mm.h>
#endif

#include "esp32_ai_memory.h"

/* Count of SRAM-tier allocations that could not use the kernel heap and
 * fell back to the user heap.  Incremented under no lock: the application
 * is single-core for now and this is diagnostic data only.
 */
static unsigned long g_sram_fallbacks;

void *esp32_ai_mem_alloc(int tier, size_t n)
{
  void *p = NULL;

  if (tier == ESP32_AI_MEM_SRAM)
    {
#ifdef CONFIG_MM_KERNEL_HEAP
      /* Kernel heap = internal SRAM in the AI board configuration. */
      p = kmm_memalign(sizeof(void *), n);
      if (p == NULL)
        {
          /* Fall back visibly: a "SRAM" label would otherwise be wrong. */
          g_sram_fallbacks++;
          p = memalign(sizeof(void *), n);
        }
#else
      p = memalign(sizeof(void *), n);
#endif
    }
  else
    {
      /* User heap; with CONFIG_ESPRESSIF_SPIRAM_USER_HEAP this is PSRAM. */
      p = memalign(sizeof(void *), n);
    }

  return p;
}

void esp32_ai_mem_free(void *p)
{
  free(p);
}

size_t esp32_ai_mem_tier_free(int tier)
{
  struct mallinfo info;

  if (tier == ESP32_AI_MEM_SRAM)
    {
#ifdef CONFIG_MM_KERNEL_HEAP
      /* kmm_mallinfo() is callable directly in flat builds. */
      info = kmm_mallinfo();
      return (size_t)info.fordblks;
#else
      info = mallinfo();
      return (size_t)info.fordblks;
#endif
    }

  info = mallinfo();
  return (size_t)info.fordblks;
}

int esp32_ai_mem_psram_present(void)
{
#ifdef CONFIG_ESPRESSIF_SPIRAM_USER_HEAP
  struct mallinfo info = mallinfo();
  size_t arena = (size_t)info.arena;

  /* The user heap arena is the whole PSRAM when SPIRAM_USER_HEAP is set;
   * if the PSRAM failed to init the arena stays at the SRAM size, which is
   * far below the expected PSRAM capacity.
   */
  if (arena >= (size_t)CONFIG_ESP32_AI_EXPECT_PSRAM_BYTES)
    {
      return 1;
    }
#endif

  return 0;
}

unsigned long esp32_ai_mem_sram_fallbacks(void)
{
  return g_sram_fallbacks;
}
