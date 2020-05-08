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
              $(patsubst %.c, %.o,          $(wildcard *.c))
X86_32     := $(patsubst %.$(ASM_EXT), %.o, $(wildcard x86/32/*.$(ASM_EXT))) \
              $(patsubst %.c, %.o,          $(wildcard x86/32/*.c))
BOOT       := $(patsubst %.$(ASM_EXT), %.o, $(wildcard x86/boot/*.$(ASM_EXT))) \
              $(patsubst %.c, %.o,          $(wildcard x86/boot/*.c))
CMD        := $(patsubst %.c, %.o,          $(wildcard cmd/*.c))
SYS        := $(patsubst %.c, %.o,          $(wildcard sys/*.c))
LIB        := $(patsubst %.c, %.o,          $(wildcard lib/*.c))
TEST       := $(patsubst %.c, %.o,          $(wildcard test/*.c))
FONT       := sys/font.o

OBJECTS    += $(X86_32) $(BOOT) $(TEST) $(CMD) $(SYS) $(LIB) $(FONT)

ifneq (CONFIG_TEST,$(findstring CONFIG_TEST,$(OPTS)))
OBJECTS    := $(filter-out $(TEST), $(OBJECTS))
endif

ifneq (CONFIG_SERIAL,$(findstring CONFIG_SERIAL,$(OPTS)))
OBJECTS    := $(filter-out sys/serial.o, $(OBJECTS))
endif

ifneq (CONFIG_FRAMEBUFFER_RGB,$(findstring CONFIG_FRAMEBUFFER_RGB,$(OPTS)))
OBJECTS    := $(filter-out sys/font.o, $(OBJECTS))
endif

OBJDEPS    := $(patsubst %.o, %.d, $(OBJECTS))

.PHONY: clean cov-configure tags

odyssey: $(OBJECTS) tags
ifeq (, $(shell which $(LD)))
	$(error $(LD) not found)
endif
	$(LD) -o odyssey $(OBJECTS) $(LDFLAGS)

sys/font.o:
	$(OBJCOPY) -O elf32-i386 -B i386 -I binary sys/f.sfn sys/font.o

%.o: %.c
ifeq (, $(shell which $(CC)))
	$(error $(CC) not found)
endif
	$(CC) $(ARCHFLAGS) $(CFLAGS) -c $< -o $@

# Compile either .asm or .S files depending on the choice of assembler
ifneq (,$(findstring nasm,$(AS)))
%.o: %.asm
  ifeq (, $(shell which $(AS)))
	$(error $(AS) not found)
  endif
	$(AS) $(ASFLAGS) $(OPTS) -o $@ $<
else
%.o: %.S
  ifeq (, $(shell which $(AS)))
	$(error $(AS) not found)
  endif
	$(AS) $(CFLAGS) -o $@ $<
endif

tags:
	ctags -R .

clean:
	-rm -f $(OBJECTS) $(OBJDEPS) odyssey

distclean: clean
	-rm -f tags

cov-configure:
ifeq (, $(shell which cov-configure))
	$(error cov-configure is not available in the PATH)
endif
	cov-configure -co $(CC) -- -ffreestanding

-include $(OBJECTS:.o=.d)
