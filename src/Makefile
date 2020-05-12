# Default ARCH to x86
ARCH := $(or $(ARCH),x86)

ifeq ($(ARCH),x86)
	include x86/x86.mk
endif

ifeq ($(ARCH),arm)
	include arm/arm.mk
endif

.PHONY: cov-configure tags

cov-configure:
ifeq (, $(shell which cov-configure))
	$(error cov-configure is not available in the PATH)
endif
	cov-configure -co $(CC) -- -ffreestanding

tags:
	ctags -R .
