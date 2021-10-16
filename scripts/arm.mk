# SPDX-FileCopyrightText: 2020 Anuradha Weeraman <anuradha@weeraman.com>
# SPDX-License-Identifier: GPL-3.0-or-later

OPTS_LIST  := $(shell grep -v "^\#" config/kernel.cfg)
OPTS       += $(foreach opt, $(OPTS_LIST), -D$(opt)) -DARCH_ARM
CFLAGS     += $(OPTS)

BOOT       := $(patsubst %.s, %.o,          $(wildcard arch/arm/boot/*.s)) \
              $(patsubst %.c, %.o,          $(wildcard arch/arm/boot/*.c))
DRV        := $(patsubst %.c, %.o,          $(wildcard arch/arm/drv/*.c))
SYS        := $(patsubst %.c, %.o,          $(wildcard sys/*.c))
TEST       := $(patsubst %.c, %.o,          $(wildcard test/*.c))
LIB        := $(patsubst %.c, %.o,          $(wildcard lib/*.c))
CMD        := $(patsubst %.c, %.o,          $(wildcard cmd/*.c))
MM         := $(patsubst %.c, %.o,          $(wildcard mm/*.c))

OBJECTS    += $(BOOT) $(DRV) $(TEST) $(LIB) $(MM) $(SYS) $(CMD)

OBJDEPS    := $(patsubst %.o, %.d, $(OBJECTS))

odyssey: $(OBJECTS)
ifeq (, $(shell command -v $(LD)))
	$(error $(LD) not found)
endif
	$(LD) $(OBJECTS) -o $@ $(LDFLAGS)
	$(OBJCOPY) -O binary $@ $@.bin

%.o: %.c
ifeq (, $(shell command -v $(CC)))
	$(error $(CC) not found)
endif
	$(CC) $(CFLAGS) -c $< -o $@

%.o: %.s
ifeq (, $(shell command -v $(AS)))
	$(error $(AS) not found)
endif
	$(AS) $(ASFLAGS) -o $@ $<

-include $(OBJECTS:.o=.d)
