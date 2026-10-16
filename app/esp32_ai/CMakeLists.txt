# Contest 2026 team 378 esp32_ai
#
# NOTE: The Make path is the validated build path for this board (see
# board/contest_board and the deployment plan).  This CMakeLists.txt is a
# best-effort stub only; the P4 CMake path has NOT been verified and must
# not be claimed as supported.
if(CONFIG_LVX_USE_DEMO_CONTEST2026_378_ESP32_AI)
  nuttx_add_application(
    NAME esp32_ai
    SRCS esp32_ai_main.c
         platform/esp32_ai_memory.c
         platform/esp32_ai_storage.c
    INCLUDE_DIRECTORIES runtime platform
    STACKSIZE 16384)
endif()