# SPDX-FileCopyrightText: 2020 Anuradha Weeraman <anuradha@weeraman.com>
# SPDX-License-Identifier: GPL-3.0-or-later

# Look for .S files when gcc is used as the assembler
ifneq (,$(findstring nasm,$(AS)))
  ASM_EXT := asm
else
  ASM_EXT := S
endif

OPTS_LIST  := $(shell grep -v "^\#" config/kernel.cfg)
OPTS       += $(foreach opt, $(OPTS_LIST), -D$(opt)) -DARCH_X86
CFLAGS     += $(OPTS)

X86_32     := $(patsubst %.$(ASM_EXT), %.o, $(wildcard arch/x86/32/*.$(ASM_EXT))) \
              $(patsubst %.c, %.o,          $(wildcard arch/x86/32/*.c))
DRV        := $(patsubst %.c, %.o,          $(wildcard arch/x86/drv/*.c))
BOOT       := $(patsubst %.$(ASM_EXT), %.o, $(wildcard arch/x86/boot/*.$(ASM_EXT))) \
              $(patsubst %.c, %.o,          $(wildcard arch/x86/boot/*.c))
SYS        := $(patsubst %.c, %.o,          $(wildcard sys/*.c))
TEST       := $(patsubst %.c, %.o,          $(wildcard test/*.c))
LIB        := $(patsubst %.c, %.o,          $(wildcard lib/*.c))
CMD        := $(patsubst %.c, %.o,          $(wildcard cmd/*.c))
MM         := $(patsubst %.c, %.o,          $(wildcard mm/*.c))
FONT       := sys/fnt.o

OBJECTS    += $(BOOT) $(X86_32) $(DRV) $(MM) $(TEST) $(CMD) $(SYS) $(LIB) $(FONT)

ifneq (CONFIG_TEST,$(findstring CONFIG_TEST,$(OPTS)))
OBJECTS    := $(filter-out $(TEST), $(OBJECTS))
endif

ifneq (CONFIG_SERIAL,$(findstring CONFIG_SERIAL,$(OPTS)))
OBJECTS    := $(filter-out sys/serial.o, $(OBJECTS))
endif

ifneq (CONFIG_FRAMEBUFFER_RGB,$(findstring CONFIG_FRAMEBUFFER_RGB,$(OPTS)))
OBJECTS    := $(filter-out sys/fnt.o, $(OBJECTS))
endif

# Clang specific arguments
ifneq (,$(findstring clang,$(CC)))
	EXTRAFLAGS += $(CLANGFLAGS)
endif

OBJDEPS    := $(patsubst %.o, %.d, $(OBJECTS))

.PHONY: modules

##     odyssey: build the odyssey kernel
odyssey: $(OBJECTS) modules
ifeq (, $(shell which $(LD)))
	$(error $(LD) not found)
endif
	$(LD) -o $@ $(OBJECTS) $(LDFLAGS)

##     modules: build the GRUB boot modules
modules:
ifneq (,$(findstring nasm,$(AS)))
	$(AS) $(NASMFLAGS_BIN) -o modules/canary.bin modules/canary.asm
else
	$(AS) $(CFLAGS) -o modules/canary.o modules/canary.S
	$(OBJCOPY) -O binary modules/canary.o modules/canary.bin
endif

sys/fnt.o: sys/fnt.sfn
	$(OBJCOPY) -O elf32-i386 -B i386 -I binary $^ $@

%.o: %.c
ifeq (, $(shell which $(CC)))
	$(error $(CC) not found)
endif
	$(CC) $(CFLAGS) $(EXTRAFLAGS) -c $< -o $@

# Compile either .asm or .S files depending on the choice of assembler
ifneq (,$(findstring nasm,$(AS)))
%.o: %.asm
  ifeq (, $(shell which $(AS)))
	$(error $(AS) not found)
  endif
	$(AS) $(NASMFLAGS_ELF) $(OPTS) -o $@ $<
else # GNU as
%.o: %.S
  ifeq (, $(shell which $(AS)))
	$(error $(AS) not found)
  endif
	$(AS) $(CFLAGS) -o $@ $<
endif # !nasm

-include $(OBJECTS:.o=.d)
