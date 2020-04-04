CC      = gcc
AS      = as
LD      = ld
OBJECTS = boot.o print.o io.o serial.o kernel.o
CFLAGS  = -m32 -nostdlib -nostdinc -fno-builtin -fno-stack-protector \
          -nostartfiles -nodefaultlibs -Wall -Wextra
LDFLAGS = -m elf_i386 -T linker.ld
ASFLAGS = --32
NPROCS  = $(shell grep -c ^processor /proc/cpuinfo)
QEMU    = qemu-system-x86_64
ISO     = kernel.iso
CBROM   = coreboot/build/coreboot.rom
EFIBIOS = /usr/share/ovmf/OVMF.fd

.PHONY: clean iso boot boot-efi boot-coreboot build-coreboot

all: kernel iso

kernel: $(OBJECTS)
	$(LD) $(LDFLAGS) -o kernel $(OBJECTS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

%.o: %.s
	$(AS) $(ASFLAGS) -o $@ $<

clean:
	-rm -f $(OBJECTS) kernel
	-rm -rf $(ISO) iso

iso: kernel
	mkdir -p iso/boot/grub/
	cp grub.cfg iso/boot/grub/
	cp kernel iso/boot/
	grub-mkrescue -o $(ISO) iso

boot: iso
	$(QEMU) -serial stdio -cdrom $(ISO)

boot-efi: iso
	$(QEMU) -serial stdio -bios $(EFIBIOS) -cdrom $(ISO)

boot-coreboot: iso $(CBROM)
	$(QEMU) -serial stdio -bios $(CBROM) -cdrom $(ISO)

build-coreboot:
	[ -e coreboot/ ] || git clone git@github.com:coreboot/coreboot.git
	cp coreboot.cfg coreboot/
	make -C coreboot/ crossgcc-i386 CPUS=$(NPROCS)
	make -C coreboot/ -j$(NPROCS)
