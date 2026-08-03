ifeq ($(and $(strip $(DEVKITPRO)),$(strip $(DEVKITARM))),)
$(error Make sure DEVKITPRO and DEVKITARM are correctly set in your environment.)
endif

PROJECT		:= gd_adv
ROM_TITLE	:= GD_ADV
ROM_GAMECODE	:= 1XXX
ROM_MAKERCODE	:=
ROM_VERSION	:=

# Binary files to process with bin2s - ADPCM music, Max only for SFX now
BINARY_FILES	:= audio/generated/*.gdaa

# Audio files to process with mmutil - Maxmod now only for SFX
AUDIO_FILES	:= audio/Sfx/*.wav

GRAPHICS	:=
SOURCES		:= graphics/*.s $(shell find levels -name '*.s') $(shell find levels -name '*.c') $(shell find src -name '*.c') src/*.s lib/*.s $(shell find graphics -name '*.c')
INCLUDES	:= include include/menus
LIBDIRS		:= $(DEVKITPRO)/libgba $(DEVKITPRO)/libtonc
LIBS		:= mm tonc gba
BUILDDIR	:= build
ALLFLAGS	:= -Wall -Wextra -g0 -gdwarf-4 -ffunction-sections -fdata-sections -masm-syntax-unified -D_DEFAULT_SOURCE
CFLAGS		:= -std=c99 -mcpu=arm7tdmi -mtune=arm7tdmi -mthumb-interwork -O2 -fomit-frame-pointer -fno-strict-aliasing -ffunction-sections -fdata-sections -Wall -Wextra
CXXFLAGS	:= -std=c++20 -fno-rtti -fno-exceptions
ASFLAGS		:=
LDFLAGS		:= -mthumb-interwork -Wl,--gc-sections $(if $(filter %_mb,$(PROJECT)),-specs=gba_mb.specs,-specs=gba.specs)
include build.mk
