/****************************************************************************
 * Contest 2026 team 378 esp32_ai - platform/esp32_ai_hash.h
 *
 * FNV-1a 32-bit fingerprint of the model image.
 *
 * The original Barista firmware prints this as "fp".  It is NOT SHA-256:
 * it is a fast device-side integrity/provenance check.  SHA-256 of the
 * release asset is recorded host-side (CONFIG_ESP32_AI_MODEL_SHA256) and
 * the two must not be conflated (see the deployment plan).
 ****************************************************************************/

#ifndef ESP32_AI_HASH_H
#define ESP32_AI_HASH_H

#include <stdint.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C"
{
#endif

#define ESP32_AI_FNV1A_OFFSET 2166136261u
#define ESP32_AI_FNV1A_PRIME  16777619u

static inline uint32_t esp32_ai_fnv1a(const uint8_t *data, size_t len)
{
  uint32_t h = ESP32_AI_FNV1A_OFFSET;
  size_t i;

  for (i = 0; i < len; i++)
    {
      h ^= data[i];
      h *= ESP32_AI_FNV1A_PRIME;
    }

  return h;
}

#ifdef __cplusplus
}
#endif

#endif /* ESP32_AI_HASH_H */