boot: kernel
	qemu-system-i386 -kernel kernel

kernel: boot.o kernel.o
	ld -m elf_i386 -T linker.ld -o kernel boot.o kernel.o

boot.o:
	as --32 -o boot.o boot.s

kernel.o:
	gcc -m32 -o kernel.o -c kernel.c

clean:
	rm *.o kernel
