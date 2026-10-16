############################################################################
# Contest 2026 team 378 esp32_ai - Makefile
#
# Make-path only.  The CMake path is NOT validated for this board (see the
# deployment plan); CMakeLists.txt is kept only as a stub for a future
# port and must not be claimed as supported.
############################################################################

include $(APPDIR)/Make.defs

PROGNAME  = esp32_ai
PRIORITY  = SCHED_PRIORITY_DEFAULT
STACKSIZE = 16384
MODULE    = $(CONFIG_LVX_USE_DEMO_CONTEST2026_378_ESP32_AI)

MAINSRC = esp32_ai_main.c

# The runtime and platform layers are header-only (llm.h, bpe_tokenizer.h)
# or small C files in the platform/ directory.
CSRCS = \
  platform/esp32_ai_memory.c \
  platform/esp32_ai_storage.c

# Generated Barista resource headers are mandatory for the full Q&A build.
# Enforce presence at build time so a stale/incomplete tree fails loudly
# instead of producing a binary that answers nonsense.
ifeq ($(CONFIG_ESP32_AI_BARISTA_RESOURCES),y)
GENERATED := $(wildcard generated/tokenizer_encoder.h generated/barista_words.h generated/barista_out2in.h)
ifneq ($(words $(GENERATED)),3)
$(error esp32_ai: CONFIG_ESP32_AI_BARISTA_RESOURCES is set but generated/ contains no tokenizer_encoder.h/barista_words.h/barista_out2in.h. Run tools/ to generate them from the verified model release before enabling this option.)
endif
endif

# Include paths for the runtime and platform headers.
CFLAGS += ${INCDIR_PREFIX}$(CURDIR)$(DELIM)runtime
CFLAGS += ${INCDIR_PREFIX}$(CURDIR)$(DELIM)platform

include $(APPDIR)/Application.mk