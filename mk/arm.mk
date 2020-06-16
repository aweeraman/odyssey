# SPDX-FileCopyrightText: 2020 Anuradha Weeraman <anuradha@weeraman.com>
# SPDX-License-Identifier: GPL-3.0-or-later

# Include debug symbols during build
ifeq ($(DEBUG),yes)
  CFLAGS     += -g
  ASFLAGS    += -g
endif

OPTS_LIST  := $(shell grep -v "^\#" config/kernel.cfg)
OPTS       += $(foreach opt, $(OPTS_LIST), -D$(opt)) -DARCH_ARM
CFLAGS     += $(OPTS)

BOOT       := $(patsubst %.s, %.o,          $(wildcard src/arm/boot/*.s)) \
              $(patsubst %.c, %.o,          $(wildcard src/arm/boot/*.c))
DRV        := $(patsubst %.c, %.o,          $(wildcard src/arm/drv/*.c))
LIB        := $(patsubst %.c, %.o,          $(wildcard src/lib/*.c))
SYS        := $(patsubst %.c, %.o,          $(wildcard src/sys/*.c))
MM         := $(patsubst %.c, %.o,          $(wildcard src/mm/*.c))
TEST       := $(patsubst %.c, %.o,          $(wildcard test/*.c))

OBJECTS    += $(BOOT) $(DRV) $(TEST) $(LIB) $(MM) $(SYS)

OBJDEPS    := $(patsubst %.o, %.d, $(OBJECTS))

odyssey: $(OBJECTS)
ifeq (, $(shell which $(LD)))
	$(error $(LD) not found)
endif
	$(LD) $(OBJECTS) -o odyssey $(LDFLAGS)
	$(OBJCOPY) -O binary odyssey odyssey.bin

%.o: %.c
ifeq (, $(shell which $(CC)))
	$(error $(CC) not found)
endif
	$(CC) $(CFLAGS) -c $< -o $@

%.o: %.s
ifeq (, $(shell which $(AS)))
	$(error $(AS) not found)
endif
	$(AS) $(ASFLAGS) -o $@ $<

-include $(OBJECTS:.o=.d)
