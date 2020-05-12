ARCH := $(or $(ARCH),x86)
ifeq ($(ARCH),x86)
	CONFIG := config/build_x86.cfg
endif
ifeq ($(ARCH),arm)
	CONFIG := config/build_arm.cfg
endif
include $(CONFIG)

ifeq ($(DEBUG),yes)
	QEMU_ARGS += -s -S
endif

MAJOR_VERSION := $(shell grep CONFIG_VERSION_MAJOR config/kernel.cfg  | cut -d"=" -f2)
MINOR_VERSION := $(shell grep CONFIG_VERSION_MINOR config/kernel.cfg  | cut -d"=" -f2)
VERSION       := "$(MAJOR_VERSION).$(MINOR_VERSION)"

.PHONY: all clean iso boot boot-coreboot boot-efi deps coverity coverity-submit

all:
	$(MAKE) ARCH=$(ARCH) -j $(NPROC) -C src

clean:
	-$(MAKE) -C src clean
	-rm -rf $(ISO) iso
	-rm -f odyssey-coverity.tar.gz
	-rm -rf cov-int
	-rm -f odyssey.img flash.bin

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

img:
	mkimage -A arm -O linux -T kernel -a 0x0080000000 -e 0x0080000000 -C none -d src/odyssey.bin odyssey.img
	cat deps/u-boot/u-boot odyssey.img > flash.bin

iso:
	mkdir -p iso/boot/grub/
	cp config/grub.cfg iso/boot/grub/
	cp src/odyssey iso/boot/
	grub-mkrescue -o $(ISO) iso

boot:
ifeq ($(ARCH),x86)
	$(MAKE) all
	$(MAKE) iso
	$(QEMU) $(QEMU_ARGS) -m size=$(MEMORY) -serial stdio -cdrom $(ISO)
endif
ifeq ($(ARCH),arm)
	$(MAKE) all
	$(MAKE) img
	$(QEMU) $(QEMU_ARGS) -m size=$(MEMORY) -kernel src/odyssey
endif

boot-uboot:
ifeq ($(ARCH),arm)
	$(QEMU) $(QEMU_ARGS) -m size=$(MEMORY) -kernel deps/u-boot/u-boot
endif

boot-coreboot: iso $(CBROM)
ifeq ($(ARCH),x86)
	$(QEMU) $(QEMU_ARGS) -m size=$(MEMORY) -serial stdio -bios $(CBROM) -cdrom $(ISO)
endif

boot-efi: iso
ifeq ($(ARCH),x86)
	# Qemu hangs when specifying the memory argument
	$(QEMU) $(QEMU_ARGS) -serial stdio -bios $(EFIBIOS) -cdrom $(ISO)
endif

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
