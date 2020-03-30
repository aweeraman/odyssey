kernel: boot.o kernel.o
	ld -m elf_i386 -T linker.ld -o kernel boot.o kernel.o
	cp kernel cdrom/boot/kernel

boot.o:
	as --32 -o boot.o boot.s

kernel.o:
	gcc -m32 -o kernel.o -c kernel.c

clean:
	rm *.o kernel cdrom.iso cdrom/boot/kernel

iso: kernel
	grub-mkrescue -o image.iso cdrom

boot: iso
	qemu-system-x86_64 -cdrom image.iso

boot-efi: iso
	qemu-system-x86_64 -bios /usr/share/ovmf/OVMF.fd -cdrom image.iso

boot-coreboot: iso
	qemu-system-i386 -bios coreboot/build/coreboot.rom -cdrom image.iso
