CONFIG := config/build.cfg
include $(CONFIG)

ifeq ($(DEBUG),yes)
	QEMU_ARGS += -s -S
endif

.PHONY: all clean iso boot boot-efi boot-coreboot build-coreboot coverity coverity-submit

all:
	$(MAKE) -j $(NPROC) -C src

clean:
	-$(MAKE) -C src clean
	-rm -rf $(ISO) iso
	-rm -f minos-coverity.tar.gz
	-rm -rf cov-int

coverity:
	cov-build --dir cov-int $(MAKE) all
	tar zcvf minos-coverity.tar.gz cov-int

coverity-submit: coverity
	curl --form token=$(COVERITY_TOKEN) \
	--form email=anuradha@weeraman.com \
	--form file=@minos-coverity.tar.gz \
	--form version="0.1" \
	--form description="An experimental x86 operating system" \
	https://scan.coverity.com/builds?project=minos

iso: all
	mkdir -p iso/boot/grub/
	cp config/grub.cfg iso/boot/grub/
	cp src/kernel iso/boot/
	grub-mkrescue -o $(ISO) iso

boot: iso
	$(QEMU_32) $(QEMU_ARGS) -m size=$(MEMORY) -serial stdio -cdrom $(ISO)

boot-coreboot: iso $(CBROM)
	$(QEMU_32) $(QEMU_ARGS) -m size=$(MEMORY) -serial stdio -bios $(CBROM) -cdrom $(ISO)

boot-efi: iso
	# Qemu hangs when specifying the memory argument
	# Cannot attach gdb to qemu_64 as the binaries are built for i386
	$(QEMU_64) -serial stdio -bios $(EFIBIOS) -cdrom $(ISO)

build-coreboot:
	[ -e coreboot/ ] || git clone git@github.com:coreboot/coreboot.git
	cp config/coreboot.cfg coreboot/.config
	make -C coreboot/ crossgcc-i386 CPUS=$(NPROCS)
	make -C coreboot/ -j$(NPROCS)
