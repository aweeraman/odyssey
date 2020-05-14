# Default ARCH to x86
ARCH := $(or $(ARCH),x86)
include $(ARCH)/$(ARCH).mk

.PHONY: cov-configure tags

cov-configure:
ifeq (, $(shell which cov-configure))
	$(error cov-configure is not available in the PATH)
endif
	cov-configure -co $(CC) -- -ffreestanding

tags:
	ctags -R .
