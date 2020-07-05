# SPDX-FileCopyrightText: 2020 Anuradha Weeraman <anuradha@weeraman.com>
# SPDX-License-Identifier: GPL-3.0-or-later

.PHONY: image iso boot boot-uboot boot-coreboot boot-efi

ifeq ($(DEBUG),yes)
	QEMU_ARGS += -s -S
endif

##     image: build the ARM binary image
image: odyssey
	mkimage -A arm -O linux -T kernel -a 0x0082000000 -e 0x0082000000 \
		-C none -d odyssey.bin odyssey.img

##     iso: build the x86 ISO image
iso: odyssey
	mkdir -p iso/boot/grub/
	cp config/grub.cfg iso/boot/grub/
	cp odyssey iso/boot/
	mkdir -p iso/modules
	cp modules/canary.bin iso/modules/
	grub-mkrescue -o $(ISO) iso

##     boot: launch Odyssey in qemu (use ARCH=arm, ARCH=x86 to specify architecture)
boot: odyssey
ifeq ($(ARCH),arm)
	$(MAKE) ARCH=$(ARCH) image
	$(QEMU) $(QEMU_ARGS) -m size=$(MEMORY) -kernel odyssey
else ifeq ($(ARCH),x86)
	$(MAKE) ARCH=$(ARCH) iso
	$(QEMU) $(QEMU_ARGS) -m size=$(MEMORY) -serial stdio -cdrom $(ISO)
endif

##     boot-uboot: boot the ARM image using qemu/u-boot
boot-uboot: image
	$(QEMU) $(QEMU_ARGS) -m size=$(MEMORY) -kernel deps/u-boot/u-boot

##     boot-coreboot: boot the x86 ISO using qemu/coreboot
boot-coreboot: iso $(CBROM)
	$(QEMU) $(QEMU_ARGS) -m size=$(MEMORY) -serial stdio -bios $(CBROM) -cdrom $(ISO)

##     boot-efi: boot the x86 ISO using qemu_64/EFI
boot-efi: iso
	# Qemu hangs when specifying the memory argument
	$(QEMU_64) $(QEMU_ARGS) -serial stdio -bios $(EFIBIOS) -cdrom $(ISO)
