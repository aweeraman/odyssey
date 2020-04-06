QEMU      := qemu-system-x86_64
MEMORY    := 32
ISO       := kernel.iso
CBROM     := coreboot/build/coreboot.rom
EFIBIOS   := /usr/share/ovmf/OVMF.fd
NPROCS    := $(shell grep -c ^processor /proc/cpuinfo)

.PHONY: all clean iso boot boot-efi boot-coreboot build-coreboot

all:
	+$(MAKE) -C src

clean:
	+$(MAKE) -C src clean
	-rm -rf $(ISO) iso

iso: all
	mkdir -p iso/boot/grub/
	cp config/grub.cfg iso/boot/grub/
	cp src/kernel iso/boot/
	grub-mkrescue -o $(ISO) iso

boot: iso
	$(QEMU) -m size=$(MEMORY) -serial stdio -cdrom $(ISO)

boot-efi: iso
	# Qemu hangs when specifying the memory argument
	$(QEMU) -serial stdio -bios $(EFIBIOS) -cdrom $(ISO)

boot-coreboot: iso $(CBROM)
	$(QEMU) -m size=$(MEMORY) -serial stdio -bios $(CBROM) -cdrom $(ISO)

build-coreboot:
	[ -e coreboot/ ] || git clone git@github.com:coreboot/coreboot.git
	cp config/coreboot.cfg coreboot/.config
	make -C coreboot/ crossgcc-i386 CPUS=$(NPROCS)
	make -C coreboot/ -j$(NPROCS)
