NPROCS:=$(shell grep -c ^processor /proc/cpuinfo)

.PHONY: clean iso boot boot-efi boot-coreboot build-coreboot

kernel: boot.o kernel.o
	ld -m elf_i386 -T linker.ld -o kernel boot.o kernel.o

boot.o: boot.s
	as --32 -o boot.o boot.s

kernel.o: kernel.c
	gcc -m32 -o kernel.o -c kernel.c

clean:
	-rm -f *.o kernel
	-rm -rf image.iso iso

iso: kernel
	mkdir -p iso/boot/grub/
	cp grub.cfg iso/boot/grub/
	cp kernel iso/boot/
	grub-mkrescue -o image.iso iso

boot: iso
	qemu-system-x86_64 -cdrom image.iso

boot-efi: iso
	qemu-system-x86_64 -bios /usr/share/ovmf/OVMF.fd -cdrom image.iso

boot-coreboot: iso coreboot/build/coreboot.rom
	qemu-system-i386 -bios coreboot/build/coreboot.rom -cdrom image.iso

build-coreboot:
	[ -e coreboot/ ] || git clone git@github.com:coreboot/coreboot.git
	cp coreboot.cfg coreboot/
	make -C coreboot/ crossgcc-i386 CPUS=$(NPROCS)
	make -C coreboot/ -j$(NPROCS)
