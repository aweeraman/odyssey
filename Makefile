# SPDX-FileCopyrightText: 2020 Anuradha Weeraman <anuradha@weeraman.com>
# SPDX-License-Identifier: GPL-3.0-or-later

# Default ARCH is x86
ARCH := $(or $(ARCH),x86)

-include config/build_$(ARCH).cfg

ifeq ($(DEBUG),yes)
	QEMU_ARGS += -s -S
endif

.PHONY: clean tags

-include mk/$(ARCH).mk

tags:
	ctags -R src

clean:
	-rm -rf $(ISO) iso odyssey.img odyssey-coverity.tar.gz cov-int
	-rm -f odyssey odyssey.map odyssey.bin
	-find src \( -name '*.o' -o -name '*.d' -o -name '*.bin' \) -exec rm {} \;
	-rm -f tags

-include mk/qemu.mk
-include mk/deps.mk
-include mk/coverity.mk
