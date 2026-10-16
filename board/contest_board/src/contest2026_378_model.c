/****************************************************************************
 * vendor/openvela/boards/contest2026_378_board/src/contest2026_378_model.c
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Board-level model storage region (see contest2026_378_model.h).
 ****************************************************************************/

#include <nuttx/config.h>

#include <sys/types.h>
#include <errno.h>
#include <syslog.h>
#include <stdio.h>

#include <nuttx/mtd/mtd.h>

#ifdef CONFIG_ESPRESSIF_SPIFLASH
#  include "espressif/esp_spiflash.h"
#  include "espressif/esp_spiflash_mtd.h"
#endif

#include "contest2026_378_model.h"

/* Fallbacks so this file still computes cleanly when the app Kconfig is
 * not present in the build (config symbols default to 0/empty).
 */
#ifndef CONFIG_ESP32_AI_MODEL_FLASH_OFFSET
#  define CONFIG_ESP32_AI_MODEL_FLASH_OFFSET 0
#endif

#ifndef CONFIG_ESP32_AI_MODEL_FLASH_SIZE
#  define CONFIG_ESP32_AI_MODEL_FLASH_SIZE 0
#endif

/****************************************************************************
 * Public Functions
 ****************************************************************************/

int contest2026_378_model_init(void)
{
#if defined(CONFIG_ESP32_AI_MODEL_STORAGE) && defined(CONFIG_ESPRESSIF_SPIFLASH)
  int ret;
  uint32_t offset = (uint32_t)CONFIG_ESP32_AI_MODEL_FLASH_OFFSET;
  uint32_t size   = (uint32_t)CONFIG_ESP32_AI_MODEL_FLASH_SIZE;
  struct mtd_dev_s *mtd;

  if (size == 0)
    {
      syslog(LOG_ERR,
             "contest2026_378_model: model storage size is 0; "
             "check CONFIG_ESP32_AI_MODEL_FLASH_SIZE\n");
      return -EINVAL;
    }

  /* Create a dedicated MTD partition over the model region. */
  mtd = esp_spiflash_alloc_mtdpart(offset, size);
  if (mtd == NULL)
    {
      syslog(LOG_ERR,
             "contest2026_378_model: failed to alloc MTD partition "
             "offset=0x%x size=0x%x\n", (unsigned)offset, (unsigned)size);
      return -ENODEV;
    }

  /* Expose it as a read-only character device.  The app opens it O_RDONLY
   * and performs bounded pread()s through the BCH proxy.
   */
  ret = register_mtddriver("/dev/model", mtd, 0444, mtd);
  if (ret < 0)
    {
      syslog(LOG_ERR,
             "contest2026_378_model: register_mtddriver(/dev/model) "
             "failed: %d\n", ret);
      return ret;
    }

  syslog(LOG_INFO, "contest2026_378_model: /dev/model over flash "
         "offset=0x%x size=0x%x (%u B)\n",
         (unsigned)offset, (unsigned)size, (unsigned)size);
  return OK;
#else
  /* Model storage not requested; nothing to do. */
  return OK;
#endif
}