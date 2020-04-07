DEBUG     := no
QEMU_32   := qemu-system-i386
QEMU_64   := qemu-system-x86_64
MEMORY    := 32
ISO       := kernel.iso
CBROM     := coreboot/build/coreboot.rom
EFIBIOS   := /usr/share/ovmf/OVMF.fd
NPROCS    := $(shell grep -c ^processor /proc/cpuinfo)

ifeq ($(DEBUG),yes)
	QEMU_ARGS += -s -S
endif

.PHONY: all clean iso boot boot-efi boot-coreboot build-coreboot

all:
	+$(MAKE) DEBUG=$(DEBUG) -C src

clean:
	+$(MAKE) -C src clean
	-rm -rf $(ISO) iso

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
