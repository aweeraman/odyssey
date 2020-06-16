# SPDX-FileCopyrightText: 2020 Anuradha Weeraman <anuradha@weeraman.com>
# SPDX-License-Identifier: GPL-3.0-or-later

ARCH   := $(or $(ARCH),x86)

-include config/build_$(ARCH).cfg

ifeq ($(DEBUG),yes)
	QEMU_ARGS += -s -S
endif

all:
	$(MAKE) ARCH=$(ARCH) -j $(NPROC) -C src

clean:
	-$(MAKE) -C src clean
	-rm -rf $(ISO) iso odyssey.img odyssey-coverity.tar.gz cov-int

image: clean all
	mkimage -A arm -O linux -T kernel -a 0x0082000000 -e 0x0082000000 \
		-C none -d src/odyssey.bin odyssey.img

iso: clean all
	mkdir -p iso/boot/grub/
	cp config/grub.cfg iso/boot/grub/
	cp src/odyssey iso/boot/
	mkdir -p iso/modules
	cp src/modules/canary.bin iso/modules/
	grub-mkrescue -o $(ISO) iso

boot:
ifeq ($(ARCH),arm)
	$(MAKE) ARCH=arm image
	$(QEMU) $(QEMU_ARGS) -m size=$(MEMORY) -kernel src/odyssey
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

coverity:
ifeq (, $(shell which cov-build))
	$(error cov-build is not available in the PATH)
endif
	$(MAKE) -C src cov-configure
	cov-build --dir cov-int $(MAKE) all
	tar zcvf odyssey-coverity.tar.gz cov-int

coverity-submit: clean coverity
ifndef COVERITY_TOKEN
	$(error COVERITY_TOKEN is not set)
endif
	curl --form token=$(COVERITY_TOKEN) \
	--form email=anuradha@weeraman.com \
	--form file=@odyssey-coverity.tar.gz \
	--form version="$(VERSION)" \
	--form description="An experimental x86 operating system" \
	https://scan.coverity.com/builds?project=minos

-include mk/deps.mk

.PHONY: all clean image iso boot boot-uboot boot-coreboot \
	boot-efi coverity coverity-submit
