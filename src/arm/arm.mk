CONFIG := ../config/build_arm.cfg
include $(CONFIG)

# Include debug symbols during build
ifeq ($(DEBUG),yes)
  CFLAGS     += -g
  ASFLAGS    += -g
endif

OPTS_LIST  := $(shell grep -v "^\#" ../config/kernel.cfg)
OPTS       += $(foreach opt, $(OPTS_LIST), -D$(opt)) -DARCH_ARM
CFLAGS     += $(OPTS)

BOOT       := $(patsubst %.s, %.o,          $(wildcard arm/boot/*.s)) \
              $(patsubst %.c, %.o,          $(wildcard arm/boot/*.c))
DRV        := $(patsubst %.c, %.o,          $(wildcard arm/drv/*.c))
TEST       := $(patsubst %.c, %.o,          $(wildcard test/*.c))
LIB        := $(patsubst %.c, %.o,          $(wildcard lib/*.c))
MM         := sys/ffmm.o

OBJECTS    += $(BOOT) $(DRV) $(TEST) $(LIB) $(MM)

OBJDEPS    := $(patsubst %.o, %.d, $(OBJECTS))

.PHONY: clean distclean

odyssey: $(OBJECTS)
ifeq (, $(shell which $(LD)))
	$(error $(LD) not found)
endif
	$(LD) $(OBJECTS) -o odyssey $(LDFLAGS)
	$(OBJCOPY) -O binary odyssey odyssey.bin

%.o: %.c
ifeq (, $(shell which $(CC)))
	$(error $(CC) not found)
endif
	$(CC) $(CFLAGS) -c $< -o $@

%.o: %.s
ifeq (, $(shell which $(AS)))
	$(error $(AS) not found)
endif
	$(AS) $(ASFLAGS) -o $@ $<

clean:
	-rm -f $(OBJECTS) $(OBJDEPS) odyssey odyssey.map odyssey.bin
	-find . -name "*.o.d" -exec rm {} \;

distclean: clean
	-rm -f tags

-include $(OBJECTS:.o=.d)
