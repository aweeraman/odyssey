CONFIG := ../config/build.cfg
include $(CONFIG)

ifeq ($(CC),clang)
	ARCHFLAGS := --target=i686-pc-none-elf -march=i686
endif

# Look for .S files when gcc is used as the assembler
ifeq ($(AS),gcc)
  ASM_EXT := S
else
  ASM_EXT := asm
endif

# Include debug symbols during build
ifeq ($(DEBUG),yes)
  CFLAGS     += -g
  ASFLAGS    += -g
endif

OPTS       := $(shell grep -v "^\#" ../config/kernel.cfg)
CFLAGS     += $(foreach opt, $(OPTS), -D$(opt))
OBJECTS    := $(patsubst %.$(ASM_EXT), %.o, $(wildcard *.$(ASM_EXT))) \
              $(patsubst %.c, %.o, $(wildcard *.c))

.PHONY: clean

kernel: $(OBJECTS)
	$(LD) $(LDFLAGS) -o kernel $(OBJECTS)

%.o: %.c
	$(CC) $(ARCHFLAGS) $(CFLAGS) -c $< -o $@

%.o: %.S
	$(AS) $(CFLAGS) -o $@ $<

%.o: %.asm
	$(AS) $(ASFLAGS) -o $@ $<

clean:
	-rm -f $(OBJECTS) *.o *.d kernel

-include $(OBJECTS:.o=.d)
