/****************************************************************************
 * Contest 2026 team 378 esp32_ai - platform/esp32_ai_storage.h
 *
 * Explicit storage-view interface for the model image.
 *
 * The Arduino sketch used esp_partition_find_first + esp_partition_mmap,
 * which do not exist (and mmap is not assumed) on this NuttX simple-boot
 * target.  This layer instead exposes bounded, read-only random access to
 * a model region in SPI flash:
 *
 *   - Backend: the board registers /dev/model as an MTD character device
 *     over CONFIG_ESP32_AI_MODEL_FLASH_OFFSET/SIZE (see
 *     board/contest_board/src/contest2026_378_model.c).  Open + pread
 *     gives random access; every read is bounds-checked against the
 *     region size so a corrupt length can never read across regions.
 *
 *   - No mmap is assumed: esp32_ai_storage_load_image() reads the whole
 *     image into a caller-provided buffer (PSRAM in the AI config).
 *     A future backend that can map flash may add a map hook without
 *     changing the callers.
 *
 * Bounds contract: this file never touches flash outside
 * [0, region_size).  All offsets/lengths are sanity checked.
 ****************************************************************************/

#ifndef ESP32_AI_STORAGE_H
#define ESP32_AI_STORAGE_H

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C"
{
#endif

typedef struct
{
  int fd;              /* open /dev/model descriptor, or -1 */
  char devpath[32];    /* device node used */
  uint32_t size;       /* region size in bytes */
  int readonly;        /* 1 while open */
} esp32_ai_storage_t;

/****************************************************************************
 * Name: esp32_ai_storage_open
 *
 * Description:
 *   Open the model storage region read-only.
 *
 * Returned Value:
 *   0 on success, -1 on failure (errno set).
 *
 ****************************************************************************/
int esp32_ai_storage_open(esp32_ai_storage_t *s);

/****************************************************************************
 * Name: esp32_ai_storage_close
 *
 * Description:
 *   Close the model storage region.  No-op if not open.
 *
 ****************************************************************************/
void esp32_ai_storage_close(esp32_ai_storage_t *s);

/****************************************************************************
 * Name: esp32_ai_storage_read
 *
 * Description:
 *   Bounded random read: copy len bytes at flash offset off into buf.
 *   Refuses any range that falls outside [0, s->size) or wraps.
 *
 * Returned Value:
 *   Number of bytes read, or -1 with errno set.
 *
 ****************************************************************************/
ssize_t esp32_ai_storage_read(esp32_ai_storage_t *s, void *buf,
                              size_t len, uint32_t off);

/****************************************************************************
 * Name: esp32_ai_storage_load_image
 *
 * Description:
 *   Read the full region (up to s->size bytes) into buf.  This is the
 *   mmap substitute: the runtime binds tensor pointers into this buffer,
 *   so it must stay allocated for the lifetime of the Model.
 *
 * Returned Value:
 *   Number of bytes read, or -1 with errno set.
 *
 ****************************************************************************/
ssize_t esp32_ai_storage_load_image(esp32_ai_storage_t *s, void *buf,
                                    size_t buflen);

#ifdef __cplusplus
}
#endif

#endif /* ESP32_AI_STORAGE_H */