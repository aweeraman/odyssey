# Default ARCH to i386
ARCH := $(or $(ARCH),i386)

ifeq ($(ARCH),i386)
	include x86/i386.mk
endif

ifeq ($(ARCH),arm)
	include arm/arm.mk
endif
