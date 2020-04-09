CONFIG := ../config/build.cfg
include $(CONFIG)

# Look for .S files when gcc is used as the assembler
ifeq ($(AS),gcc)
  ASM_EXT := S
else
  ASM_EXT := asm
endif

# Include debug symbols during build
ifeq ($(DEBUG),yes)
  ADDL_CFLAGS     += -g
  ADDL_NASM_FLAGS += -g
endif

OPTS       := $(shell grep -v "^\#" ../config/kernel.cfg)
CFLAGS     += $(foreach opt, $(OPTS), -D$(opt))
OBJECTS    := $(patsubst %.$(ASM_EXT), %.o, $(wildcard *.$(ASM_EXT))) \
              $(patsubst %.c, %.o, $(wildcard *.c))

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
