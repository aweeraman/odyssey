# SPDX-FileCopyrightText: 2020 Anuradha Weeraman <anuradha@weeraman.com>
# SPDX-License-Identifier: GPL-3.0-or-later

ARCH   := $(or $(ARCH),x86)

-include config/build_$(ARCH).cfg

ifeq ($(DEBUG),yes)
	QEMU_ARGS += -s -S
endif

.PHONY: all clean

all:
	$(MAKE) ARCH=$(ARCH) -j $(NPROC) -C src

clean:
	-$(MAKE) -C src clean
	-rm -rf $(ISO) iso odyssey.img odyssey-coverity.tar.gz cov-int

-include mk/qemu.mk
-include mk/deps.mk
-include mk/coverity.mk
