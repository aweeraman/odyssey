CONFIG := ../config/build.cfg
include $(CONFIG)

ifneq (,$(findstring clang,$(CC)))
	ARCHFLAGS := --target=i686-pc-none-elf -march=i686
endif

# Look for .S files when gcc is used as the assembler
ifneq (,$(findstring nasm,$(AS)))
  ASM_EXT := asm
else
  ASM_EXT := S
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
	$(LD) -o kernel $(OBJECTS) $(LDFLAGS)

%.o: %.c
	$(CC) $(ARCHFLAGS) $(CFLAGS) -c $< -o $@

%.o: %.S
	$(AS) $(CFLAGS) -o $@ $<

%.o: %.asm
	$(AS) $(ASFLAGS) -o $@ $<

clean:
	-rm -f $(OBJECTS) *.o *.d kernel

-include $(OBJECTS:.o=.d)
