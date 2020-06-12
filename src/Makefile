# Default ARCH to x86
ARCH := $(or $(ARCH),x86)
include $(ARCH)/$(ARCH).mk

.PHONY: cov-configure tags clean

cov-configure:
ifeq (, $(shell which cov-configure))
	$(error cov-configure is not available in the PATH)
endif
	cov-configure -co $(CC) -- -ffreestanding

clean:
	-rm -f odyssey odyssey.map odyssey.bin
	-find . \( -name '*.o' -o -name '*.d' -o -name '*.bin' \) -exec rm {} \;
	-rm -f tags

tags:
	ctags -R .
