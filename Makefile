CONFIG := config/build.cfg
include $(CONFIG)

ifeq ($(DEBUG),yes)
	QEMU_ARGS += -s -S
endif

MAJOR_VERSION := $(shell grep CONFIG_VERSION_MAJOR config/kernel.cfg  | cut -d"=" -f2)
MINOR_VERSION := $(shell grep CONFIG_VERSION_MINOR config/kernel.cfg  | cut -d"=" -f2)
VERSION       := "$(MAJOR_VERSION).$(MINOR_VERSION)"

.PHONY: all clean iso boot boot-efi boot-coreboot build-coreboot \
	      coverity coverity-submit

all:
	$(MAKE) -j $(NPROC) -C src

clean:
	-$(MAKE) -C src clean
	-rm -rf $(ISO) iso
	-rm -f minos-coverity.tar.gz
	-rm -rf cov-int

coverity:
ifeq (, $(shell which cov-build))
	$(error cov-build is not available in the PATH)
endif
	$(MAKE) -C src cov-configure
	cov-build --dir cov-int $(MAKE) all
	tar zcvf minos-coverity.tar.gz cov-int

coverity-submit: clean coverity
ifndef COVERITY_TOKEN
	$(error COVERITY_TOKEN is not set)
endif
	curl --form token=$(COVERITY_TOKEN) \
	--form email=anuradha@weeraman.com \
	--form file=@minos-coverity.tar.gz \
	--form version="$(VERSION)" \
	--form description="An experimental x86 operating system" \
	https://scan.coverity.com/builds?project=minos

iso: all
	mkdir -p iso/boot/grub/
	cp config/grub.cfg iso/boot/grub/
	cp src/kernel iso/boot/
	grub-mkrescue -o $(ISO) iso

boot: iso
	$(QEMU) $(QEMU_ARGS) -m size=$(MEMORY) -serial stdio -cdrom $(ISO)

boot-coreboot: iso $(CBROM)
	$(QEMU) $(QEMU_ARGS) -m size=$(MEMORY) -serial stdio -bios $(CBROM) -cdrom $(ISO)

boot-efi: iso
	# Qemu hangs when specifying the memory argument
	# Cannot attach gdb to qemu_64 as the binaries are built for i386
	$(QEMU) -serial stdio -bios $(EFIBIOS) -cdrom $(ISO)

build-coreboot:
	[ -e coreboot/ ] || git clone git@github.com:coreboot/coreboot.git
	cp config/coreboot.cfg coreboot/.config
	make -C coreboot/ crossgcc-i386 CPUS=$(NPROCS)
	make -C coreboot/ -j$(NPROCS)
