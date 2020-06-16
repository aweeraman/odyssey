# SPDX-FileCopyrightText: 2020 Anuradha Weeraman <anuradha@weeraman.com>
# SPDX-License-Identifier: GPL-3.0-or-later

# Default ARCH is x86
ARCH := $(or $(ARCH),x86)

-include config/build_$(ARCH).cfg

ifeq ($(DEBUG),yes)
	QEMU_ARGS += -s -S
endif

.PHONY: clean tags

-include make/$(ARCH).mk

tags:
	ctags --exclude=deps/* -R .

clean:
	-rm -rf $(ISO) iso odyssey.img odyssey-coverity.tar.gz cov-int
	-rm -f odyssey odyssey.map odyssey.bin
	-find . -not -path "./deps/*" -not -path "./.git/*" \
		\( -name '*.o' -o -name '*.d' -o -name '*.bin' \) \
		-exec rm {} \;
	-rm -f tags

-include make/qemu.mk
-include make/deps.mk
-include make/coverity.mk
