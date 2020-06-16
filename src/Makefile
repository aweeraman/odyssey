# SPDX-FileCopyrightText: 2020 Anuradha Weeraman <anuradha@weeraman.com>
# SPDX-License-Identifier: GPL-3.0-or-later

# Default ARCH is x86
ARCH := $(or $(ARCH),x86)

include $(ARCH)/$(ARCH).mk

clean:
	-rm -f odyssey odyssey.map odyssey.bin
	-find . \( -name '*.o' -o -name '*.d' -o -name '*.bin' \) -exec rm {} \;
	-rm -f tags

tags:
	ctags -R .

cov-configure:
ifeq (, $(shell which cov-configure))
	$(error cov-configure is not available in the PATH)
endif
	cov-configure -co $(CC) -- -ffreestanding

.PHONY: cov-configure tags clean
