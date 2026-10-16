/****************************************************************************
 * vendor/openvela/boards/contest2026_378_board/src/contest2026_378_model.h
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Board-level model storage region for the esp32_ai application.
 *
 * The esp32_ai app reads the LLM model through the bounded storage-view
 * layer (platform/esp32_ai_storage).[ch] of the application; the board is
 * responsible for exposing the flash region as a read-only MTD character
 * device so that the app needs no mmap and no knowledge of the flash
 * layout.  Region geometry comes from the application Kconfig:
 *
 *   CONFIG_ESP32_AI_MODEL_FLASH_OFFSET / CONFIG_ESP32_AI_MODEL_FLASH_SIZE
 *
 * Registered device: /dev/model
 *
 * Both values must be multiples of the flash sector size (4 KiB), must not
 * overlap the NuttX simple-boot image (0x2000 .. image end) or the default
 * storage MTD area, and must be validated on the real unit before a model
 * is written (deployment plan, flash capacity table).
 ****************************************************************************/

#ifndef __VENDOR_OPENVELA_BOARDS_CONTEST2026_378_BOARD_SRC_CONTEST2026_378_MODEL_H
#define __VENDOR_OPENVELA_BOARDS_CONTEST2026_378_BOARD_SRC_CONTEST2026_378_MODEL_H

/****************************************************************************
 * Public Function Prototypes
 ****************************************************************************/

/****************************************************************************
 * Name: contest2026_378_model_init
 *
 * Description:
 *   Register /dev/model as a read-only MTD partition over the model flash
 *   region.  NO-OP unless CONFIG_ESP32_AI_MODEL_STORAGE is enabled.
 *
 * Returned Value:
 *   Zero (OK) on success; a negated errno value on failure.  Failure is
 *   logged and non-fatal for the rest of the system (the app reports it
 *   at runtime).
 *
 ****************************************************************************/

int contest2026_378_model_init(void);

#endif /* __VENDOR_OPENVELA_BOARDS_CONTEST2026_378_BOARD_SRC_CONTEST2026_378_MODEL_H */