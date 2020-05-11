# Default ARCH to i386
ARCH := $(or $(ARCH),i386)

ifeq ($(ARCH),i386)
	include x86/i386.mk
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
