# SPDX-FileCopyrightText: 2020 Anuradha Weeraman <anuradha@weeraman.com>
# SPDX-License-Identifier: GPL-3.0-or-later

# Default ARCH is x86
ARCH ?= x86

.PHONY: clean tags help
.SILENT: help

-include config/build_$(ARCH).cfg
-include make/$(ARCH).mk
-include make/common.mk
-include make/qemu.mk
-include make/deps.mk
-include make/coverity.mk
