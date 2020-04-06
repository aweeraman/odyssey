CC        := gcc
LD        := ld

# Specify either 'gcc' or 'nasm' for compiling assembly programs
AS        := nasm
ifeq ($(AS),gcc)
  ASM_EXT := S
else
	ASM_EXT := asm
endif

OBJECTS   := $(patsubst %.$(ASM_EXT), %.o, $(wildcard *.$(ASM_EXT))) \
					   $(patsubst %.c, %.o, $(wildcard *.c))
ASFLAGS   := -felf
CFLAGS    := -m32 -c -nostdlib -nostdinc -fno-builtin -fno-stack-protector \
             -nostartfiles -nodefaultlibs -Wall -Wextra -ffreestanding
LDFLAGS   := -m elf_i386 -T linker.ld

QEMU      := qemu-system-x86_64
MEMORY    := 32
ISO       := kernel.iso
CBROM     := coreboot/build/coreboot.rom
EFIBIOS   := /usr/share/ovmf/OVMF.fd
NPROCS    := $(shell grep -c ^processor /proc/cpuinfo)

.PHONY: clean iso boot boot-efi boot-coreboot build-coreboot

kernel: $(OBJECTS)
	$(LD) $(LDFLAGS) -o kernel $(OBJECTS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

%.o: %.S
	$(AS) $(CFLAGS) -o $@ $<

%.o: %.asm
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
	$(QEMU) -m size=$(MEMORY) -serial stdio -cdrom $(ISO)

boot-efi: iso
	$(QEMU) -m size=$(MEMORY) -serial stdio -bios $(EFIBIOS) -cdrom $(ISO)

boot-coreboot: iso $(CBROM)
	$(QEMU) -m size=$(MEMORY) -serial stdio -bios $(CBROM) -cdrom $(ISO)

build-coreboot:
	[ -e coreboot/ ] || git clone git@github.com:coreboot/coreboot.git
	cp coreboot.cfg coreboot/
	make -C coreboot/ crossgcc-i386 CPUS=$(NPROCS)
	make -C coreboot/ -j$(NPROCS)
