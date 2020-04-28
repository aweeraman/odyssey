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

OPTS_LIST  := $(shell grep -v "^\#" ../config/kernel.cfg)
OPTS       += $(foreach opt, $(OPTS_LIST), -D$(opt))
CFLAGS     += $(OPTS)
OBJECTS    := $(patsubst %.$(ASM_EXT), %.o, $(wildcard *.$(ASM_EXT))) \
              $(patsubst %.c, %.o, $(wildcard *.c)) font.o

ifneq (CONFIG_TEST,$(findstring CONFIG_TEST,$(OPTS)))
OBJECTS  := $(filter-out test.o, $(OBJECTS))
endif

.PHONY: clean cov-configure tags

kernel: $(OBJECTS) tags
ifeq (, $(shell which $(LD)))
	$(error $(LD) not found)
endif
	$(LD) -o kernel $(OBJECTS) $(LDFLAGS)

font.o:
	$(OBJCOPY) -O elf32-i386 -B i386 -I binary unifont.sfn font.o

%.o: %.c
ifeq (, $(shell which $(CC)))
	$(error $(CC) not found)
endif
	$(CC) $(ARCHFLAGS) $(CFLAGS) -c $< -o $@

%.o: %.S
ifeq (, $(shell which $(AS)))
	$(error $(AS) not found)
endif
	$(AS) $(CFLAGS) -o $@ $<

%.o: %.asm
ifeq (, $(shell which $(AS)))
	$(error $(AS) not found)
endif
	$(AS) $(ASFLAGS) $(OPTS) -o $@ $<

tags:
	ctags -R .

clean:
	-rm -f $(OBJECTS) *.o *.d kernel

cov-configure:
ifeq (, $(shell which cov-configure))
	$(error cov-configure is not available in the PATH)
endif
	cov-configure -co $(CC) -- -ffreestanding

-include $(OBJECTS:.o=.d)
