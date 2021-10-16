# SPDX-FileCopyrightText: 2020 Anuradha Weeraman <anuradha@weeraman.com>
# SPDX-License-Identifier: GPL-3.0-or-later

# Default ARCH is x86
ARCH ?= x86

.PHONY: clean tags help
.SILENT: help

-include config/build_$(ARCH).cfg
-include scripts/$(ARCH).mk
-include scripts/common.mk
-include scripts/qemu.mk
-include scripts/deps.mk
-include scripts/coverity.mk
