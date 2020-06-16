# SPDX-FileCopyrightText: 2020 Anuradha Weeraman <anuradha@weeraman.com>
# SPDX-License-Identifier: GPL-3.0-or-later

.PHONY: image iso boot boot-uboot boot-coreboot boot-efi

image: clean odyssey
	mkimage -A arm -O linux -T kernel -a 0x0082000000 -e 0x0082000000 \
		-C none -d odyssey.bin odyssey.img

iso: odyssey
	mkdir -p iso/boot/grub/
	cp config/grub.cfg iso/boot/grub/
	cp odyssey iso/boot/
	mkdir -p iso/modules
	cp src/modules/canary.bin iso/modules/
	grub-mkrescue -o $(ISO) iso

boot: odyssey
ifeq ($(ARCH),arm)
	$(MAKE) ARCH=arm image
	$(QEMU) $(QEMU_ARGS) -m size=$(MEMORY) -kernel odyssey
else ifeq ($(ARCH),x86)
	$(MAKE) ARCH=x86 iso
	$(QEMU) $(QEMU_ARGS) -m size=$(MEMORY) -serial stdio -cdrom $(ISO)
endif

boot-uboot: image
	$(QEMU) $(QEMU_ARGS) -m size=$(MEMORY) -kernel deps/u-boot/u-boot

boot-coreboot: iso $(CBROM)
	$(QEMU) $(QEMU_ARGS) -m size=$(MEMORY) -serial stdio -bios $(CBROM) -cdrom $(ISO)

boot-efi: iso
	# Qemu hangs when specifying the memory argument
	$(QEMU) $(QEMU_ARGS) -serial stdio -bios $(EFIBIOS) -cdrom $(ISO)
