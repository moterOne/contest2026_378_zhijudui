/****************************************************************************
 * Contest 2026 team 378 esp32_ai - platform/esp32_ai_storage.c
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Read-only random access to the model region (see esp32_ai_storage.h).
 ****************************************************************************/

#include <nuttx/config.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

#include "esp32_ai_storage.h"

#ifndef CONFIG_ESP32_AI_MODEL_FLASH_SIZE
#  define CONFIG_ESP32_AI_MODEL_FLASH_SIZE 0
#endif

int esp32_ai_storage_open(esp32_ai_storage_t *s)
{
  if (s == NULL)
    {
      errno = EINVAL;
      return -1;
    }

  memset(s, 0, sizeof(*s));
  s->fd = -1;

  /* The board registers the model region as /dev/model (board bring-up,
   * CONFIG_ESP32_AI_MODEL_STORAGE).  The MTD inode is opened through the
   * BCH character proxy, which provides seekable read().
   */
  strlcpy(s->devpath, "/dev/model", sizeof(s->devpath));
  s->fd = open(s->devpath, O_RDONLY);
  if (s->fd < 0)
    {
      return -1;
    }

  s->size = (uint32_t)CONFIG_ESP32_AI_MODEL_FLASH_SIZE;
  s->readonly = 1;
  return 0;
}

void esp32_ai_storage_close(esp32_ai_storage_t *s)
{
  if (s == NULL)
    {
      return;
    }

  if (s->fd >= 0)
    {
      close(s->fd);
    }

  s->fd = -1;
  s->readonly = 0;
}

ssize_t esp32_ai_storage_read(esp32_ai_storage_t *s, void *buf,
                              size_t len, uint32_t off)
{
  ssize_t nread;

  if (s == NULL || s->fd < 0)
    {
      errno = EBADF;
      return -1;
    }

  /* Bounds check: the whole range must sit inside the region. */
  if ((uint64_t)off + len > s->size)
    {
      errno = EFAULT;
      return -1;
    }

#if defined(CONFIG_BCH) && defined(CONFIG_MTD)
  /* pread() on the BCH proxy reads at a fixed offset without moving the
   * shared file position, which keeps concurrent readers safe.
   */
  nread = pread(s->fd, buf, len, (off_t)off);
#else
  /* Fallback: lseek + read. */
  if (lseek(s->fd, (off_t)off, SEEK_SET) < 0)
    {
      return -1;
    }

  nread = read(s->fd, buf, len);
#endif

  return nread;
}

ssize_t esp32_ai_storage_load_image(esp32_ai_storage_t *s, void *buf,
                                    size_t buflen)
{
  size_t want;
  ssize_t nread;

  if (s == NULL || s->fd < 0 || buf == NULL)
    {
      errno = EINVAL;
      return -1;
    }

  want = (size_t)s->size;
  if (want > buflen)
    {
      /* Caller's buffer is smaller than the region: only the on-flash
       * image is supposed to be present; refuse partial reads rather than
       * silently truncating a model.
       */
      errno = ENOBUFS;
      return -1;
    }

  nread = esp32_ai_storage_read(s, buf, want, 0);
  return nread;
}