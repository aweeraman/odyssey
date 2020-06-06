ARCH   := $(or $(ARCH),x86)
include config/build_$(ARCH).cfg

ifeq ($(DEBUG),yes)
	QEMU_ARGS += -s -S
endif

.PHONY: all distclean image iso boot boot-uboot boot-coreboot boot-efi deps coverity coverity-submit

all:
	$(MAKE) ARCH=$(ARCH) -j $(NPROC) -C src

distclean:
	-$(MAKE) ARCH=arm -C src clean
	-$(MAKE) ARCH=x86 -C src clean
	-rm -rf $(ISO) iso odyssey.img
	-rm -rf odyssey-coverity.tar.gz cov-int

image: distclean all
	mkimage -A arm -O linux -T kernel -a 0x0082000000 -e 0x0082000000 -C none -d src/odyssey.bin odyssey.img

iso: distclean all
	mkdir -p iso/boot/grub/
	cp config/grub.cfg iso/boot/grub/
	cp src/odyssey iso/boot/
	mkdir -p iso/modules
	cp src/modules/looper.bin iso/modules/
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

deps:
	mkdir -p deps
	# Scalable fonts
	[ -e deps/scalable-font/ ] || git clone https://gitlab.com/bztsrc/scalable-font.git deps/scalable-font
	# Coreboot
	[ -e deps/coreboot/ ] || git clone --branch 4.11 https://www.github.com/coreboot/coreboot deps/coreboot
	cd deps/coreboot && git submodule update --init --checkout
	cp config/coreboot.cfg deps/coreboot/.config
	# x86 cross compiler
	$(MAKE) -C deps/coreboot/ crossgcc-i386 CPUS=$(NPROCS)
	# ARM cross compiler
	$(MAKE) -C deps/coreboot/ crossgcc-arm CPUS=$(NPROCS)
	# Coreboot ROM
	$(MAKE) -C deps/coreboot/ -j$(NPROCS)
	# u-boot
	[ -e deps/u-boot/ ] || git clone --branch v2020.04 https://github.com/u-boot/u-boot.git deps/u-boot
	$(MAKE) -C deps/u-boot vexpress_ca15_tc2_defconfig CROSS_COMPILE=$(PWD)/deps/coreboot/util/crossgcc/xgcc/bin/arm-eabi-
	$(MAKE) -C deps/u-boot all CROSS_COMPILE=$(PWD)/deps/coreboot/util/crossgcc/xgcc/bin/arm-eabi-

coverity:
ifeq (, $(shell which cov-build))
	$(error cov-build is not available in the PATH)
endif
	$(MAKE) -C src cov-configure
	cov-build --dir cov-int $(MAKE) all
	tar zcvf odyssey-coverity.tar.gz cov-int

coverity-submit: distclean coverity
ifndef COVERITY_TOKEN
	$(error COVERITY_TOKEN is not set)
endif
	curl --form token=$(COVERITY_TOKEN) \
	--form email=anuradha@weeraman.com \
	--form file=@odyssey-coverity.tar.gz \
	--form version="$(VERSION)" \
	--form description="An experimental x86 operating system" \
	https://scan.coverity.com/builds?project=minos
