ARCH := $(or $(ARCH),i386)
ifeq ($(ARCH),i386)
	CONFIG := config/build_i386.cfg
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

iso: all
	mkdir -p iso/boot/grub/
	cp config/grub.cfg iso/boot/grub/
	cp src/odyssey iso/boot/
	grub-mkrescue -o $(ISO) iso

boot: iso
ifeq ($(ARCH),i386)
	$(QEMU) $(QEMU_ARGS) -m size=$(MEMORY) -serial stdio -cdrom $(ISO)
endif
ifeq ($(ARCH),arm)
	$(QEMU) $(QEMU_ARGS) -kernel src/odyssey
endif

boot-uboot:
ifeq ($(ARCH),arm)
	$(QEMU) $(QEMU_ARGS) -kernel deps/u-boot/u-boot
endif

boot-coreboot: iso $(CBROM)
ifeq ($(ARCH),i386)
	$(QEMU) $(QEMU_ARGS) -m size=$(MEMORY) -serial stdio -bios $(CBROM) -cdrom $(ISO)
endif

boot-efi: iso
ifeq ($(ARCH),i386)
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
	# i386 cross compiler
	$(MAKE) -C deps/coreboot/ crossgcc-i386 CPUS=$(NPROCS)
	# ARM cross compiler
	$(MAKE) -C deps/coreboot/ crossgcc-arm CPUS=$(NPROCS)
	# Coreboot ROM
	$(MAKE) -C deps/coreboot/ -j$(NPROCS)
	# u-boot
	[ -e deps/u-boot/ ] || git clone --branch v2020.04 https://github.com/u-boot/u-boot.git deps/u-boot
	$(MAKE) -C deps/u-boot vexpress_ca15_tc2_defconfig CROSS_COMPILE=$(PWD)/deps/coreboot/util/crossgcc/xgcc/bin/arm-eabi-
	$(MAKE) -C deps/u-boot all CROSS_COMPILE=$(PWD)/deps/coreboot/util/crossgcc/xgcc/bin/arm-eabi-
