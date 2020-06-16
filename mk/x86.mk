# SPDX-FileCopyrightText: 2020 Anuradha Weeraman <anuradha@weeraman.com>
# SPDX-License-Identifier: GPL-3.0-or-later

ifneq (,$(findstring clang,$(CC)))
	ARCHFLAGS := --target=i686-pc-none-elf -march=i686
endif

# Look for .S files when gcc is used as the assembler
ifneq (,$(findstring nasm,$(AS)))
  ASM_EXT := asm
else
  ASM_EXT := S
endif

# Include debug symbols during build
ifeq ($(DEBUG),yes)
  CFLAGS     += -g
  ASFLAGS    += -g
endif

OPTS_LIST  := $(shell grep -v "^\#" config/kernel.cfg)
OPTS       += $(foreach opt, $(OPTS_LIST), -D$(opt)) -DARCH_X86
CFLAGS     += $(OPTS)

X86_32     := $(patsubst %.$(ASM_EXT), %.o, $(wildcard src/x86/32/*.$(ASM_EXT))) \
              $(patsubst %.c, %.o,          $(wildcard src/x86/32/*.c))
DRV        := $(patsubst %.c, %.o,          $(wildcard src/x86/drv/*.c))
SYS        := $(patsubst %.c, %.o,          $(wildcard src/sys/*.c))
BOOT       := $(patsubst %.$(ASM_EXT), %.o, $(wildcard src/x86/boot/*.$(ASM_EXT))) \
              $(patsubst %.c, %.o,          $(wildcard src/x86/boot/*.c))
CMD        := $(patsubst %.c, %.o,          $(wildcard src/cmd/*.c))
LIB        := $(patsubst %.c, %.o,          $(wildcard src/lib/*.c))
TEST       := $(patsubst %.c, %.o,          $(wildcard src/test/*.c))
MM         := $(patsubst %.c, %.o,          $(wildcard src/mm/*.c))
FONT       := src/sys/font.o

OBJECTS    += $(BOOT) $(X86_32) $(DRV) $(MM) $(TEST) $(CMD) $(SYS) $(LIB) $(FONT)

ifneq (CONFIG_TEST,$(findstring CONFIG_TEST,$(OPTS)))
OBJECTS    := $(filter-out $(TEST), $(OBJECTS))
endif

ifneq (CONFIG_SERIAL,$(findstring CONFIG_SERIAL,$(OPTS)))
OBJECTS    := $(filter-out sys/serial.o, $(OBJECTS))
endif

ifneq (CONFIG_FRAMEBUFFER_RGB,$(findstring CONFIG_FRAMEBUFFER_RGB,$(OPTS)))
OBJECTS    := $(filter-out src/sys/font.o, $(OBJECTS))
endif

OBJDEPS    := $(patsubst %.o, %.d, $(OBJECTS))

.PHONY: modules

odyssey: $(OBJECTS) modules
ifeq (, $(shell which $(LD)))
	$(error $(LD) not found)
endif
	$(LD) -o odyssey $(OBJECTS) $(LDFLAGS)

modules:
ifneq (,$(findstring nasm,$(AS)))
	$(AS) $(ASFLAGS_MOD) -o src/modules/canary.bin src/modules/canary.asm
else
	$(AS) $(CFLAGS) -o src/modules/canary.o src/modules/canary.S
	$(OBJCOPY) -O binary src/modules/canary.o src/modules/canary.bin
endif

src/sys/font.o:
	cd src/sys; ../../$(OBJCOPY) -O elf32-i386 -B i386 -I binary f.sfn font.o

%.o: %.c
ifeq (, $(shell which $(CC)))
	$(error $(CC) not found)
endif
	$(CC) $(ARCHFLAGS) $(CFLAGS) -c $< -o $@

# Compile either .asm or .S files depending on the choice of assembler
ifneq (,$(findstring nasm,$(AS)))
%.o: %.asm
  ifeq (, $(shell which $(AS)))
	$(error $(AS) not found)
  endif
	$(AS) $(ASFLAGS) $(OPTS) -o $@ $<
else # GNU as
%.o: %.S
  ifeq (, $(shell which $(AS)))
	$(error $(AS) not found)
  endif
	$(AS) $(CFLAGS) -o $@ $<
endif # !nasm

-include $(OBJECTS:.o=.d)
