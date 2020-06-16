# SPDX-FileCopyrightText: 2020 Anuradha Weeraman <anuradha@weeraman.com>
# SPDX-License-Identifier: GPL-3.0-or-later

.PHONY: deps

deps:
	mkdir -p deps

	# Scalable fonts
	[ -e deps/scalable-font/ ] || \
		git clone https://gitlab.com/bztsrc/scalable-font.git deps/scalable-font

	# Coreboot
	[ -e deps/coreboot/ ] || \
		git clone https://www.github.com/coreboot/coreboot deps/coreboot
	cd deps/coreboot && git submodule update --init --checkout --remote
	cp config/coreboot.cfg deps/coreboot/.config

	# x86 cross compiler
	$(MAKE) -C deps/coreboot/ crossgcc-i386 CPUS=$(NPROCS)
	# ARM cross compiler
	$(MAKE) -C deps/coreboot/ crossgcc-arm CPUS=$(NPROCS)
	# Coreboot ROM
	$(MAKE) -C deps/coreboot/ -j$(NPROCS)

	# u-boot
	[ -e deps/u-boot/ ] || \
		git clone --branch v2020.04 https://github.com/u-boot/u-boot.git deps/u-boot
	$(MAKE) -C deps/u-boot vexpress_ca15_tc2_defconfig \
		CROSS_COMPILE=$(PWD)/deps/coreboot/util/crossgcc/xgcc/bin/arm-eabi-
	$(MAKE) -C deps/u-boot all \
		CROSS_COMPILE=$(PWD)/deps/coreboot/util/crossgcc/xgcc/bin/arm-eabi-
