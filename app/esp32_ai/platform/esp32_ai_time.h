/****************************************************************************
 * Contest 2026 team 378 esp32_ai - platform/esp32_ai_time.h
 *
 * Microsecond monotonic clock for inference timing (replaces the Arduino
 * esp_timer_get_time() used by the original sketch).
 ****************************************************************************/

#ifndef ESP32_AI_TIME_H
#define ESP32_AI_TIME_H

#include <stdint.h>
#include <time.h>

#ifdef __cplusplus
extern "C"
{
#endif

/****************************************************************************
 * Name: esp32_ai_time_us
 *
 * Description:
 *   Microseconds since an arbitrary fixed origin (CLOCK_MONOTONIC).
 *
 ****************************************************************************/
static inline uint64_t esp32_ai_time_us(void)
{
  struct timespec ts;

  if (clock_gettime(CLOCK_MONOTONIC, &ts) != 0)
    {
      return 0;
    }

  return (uint64_t)ts.tv_sec * 1000000ull + (uint64_t)(ts.tv_nsec / 1000);
}

#ifdef __cplusplus
}
#endif

#endif /* ESP32_AI_TIME_H */