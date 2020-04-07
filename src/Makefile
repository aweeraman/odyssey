CONFIG := ../config/build.cfg
include $(CONFIG)

ifeq ($(AS),gcc)
  ASM_EXT := S
else
	ASM_EXT := asm
endif

ifeq ($(DEBUG),yes)
	ADDL_CFLAGS     += -g
	ADDL_NASM_FLAGS += -g
endif

OPTIONS := $(shell grep -v "^\#" ../config/kernel.cfg)
KERNELOPTS += $(foreach opt, $(OPTIONS), -D$(opt))

OBJECTS   := $(patsubst %.$(ASM_EXT), %.o, $(wildcard *.$(ASM_EXT))) \
					   $(patsubst %.c, %.o, $(wildcard *.c))
ASFLAGS   := -felf $(ADDL_NASM_FLAGS)
CFLAGS    := -m32 -c -nostdlib -nostdinc -fno-builtin \
             -fno-stack-protector -nostartfiles -nodefaultlibs -Wall \
             -Wextra -ffreestanding $(ADDL_CFLAGS) $(KERNELOPTS)
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
