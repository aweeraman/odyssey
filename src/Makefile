# Specify either 'gcc' or 'clang' for compiling C programs
CC        := gcc
LD        := ld

# Specify either 'gcc' or 'nasm' for compiling assembly programs
AS        := gcc

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
NPROCS    := $(shell grep -c ^processor /proc/cpuinfo)

.PHONY: clean

kernel: $(OBJECTS)
	$(LD) $(LDFLAGS) -o kernel $(OBJECTS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

%.o: %.S
	$(AS) $(CFLAGS) -o $@ $<

%.o: %.asm
	$(AS) $(ASFLAGS) -o $@ $<

clean:
	-rm -f $(OBJECTS) *.o kernel
