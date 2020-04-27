[![Coverity Scan Build Status](https://scan.coverity.com/projects/20852/badge.svg)](https://scan.coverity.com/projects/minos)

# Minos - an experimental operating system for x86

![Minos running in Qemu](https://raw.githubusercontent.com/aweeraman/minos/master/minos.png "Minos running in Qemu")

# Build and runtime dependencies

Clone repo and install build dependencies:

```
$ git clone https://github.com/aweeraman/minos.git
$ sudo apt-get install git gcc g++ make m4 bison flex bzip2 xz-utils curl zlib1g-dev gnat python
```

Build GCC cross compiler and openboot:

```
$ cd minos
$ make deps
```

Install additional dependencies required for building and running minos
in qemu:

```
$ sudo apt-get install grub-common mtools qemu-system-x86 ovmf exuberant-ctags
```

Optionally, if you wish to use clang, you can install it and configure
it in the build config as follows:

```
$ sudo apt-get install clang
```

Set the following variables in config/build.cfg to use clang:

```
CC        := clang
```

nasm is built as part of the cross compiler dependencies and can be
used in place GNU as for building the assembly programs by setting
the following in in config/build.cfg. The GNU assembler will be used
by default.

```
AS        := ../coreboot/util/crossgcc/xgcc/bin/nasm
```

# Running in qemu

To build and run in qemu with coreboot:

```
$ make boot
```

To build and run in qemu with OVMS/EFI:

```
$ make boot-efi
```

# Configuration

List capabilities to be included in the kernel in config/kernel.cfg.
Currently available options are:

```
CONFIG_VERSION_MAJOR=0
CONFIG_VERSION_MINOR=1

# Target x86 32-bit architecture
CONFIG_ARCH_X86_32

CONFIG_STACK=4096

# COM1=0x3F8,COM2=0x2F8,COM3=0x3E8,COM4=0x2E8
CONFIG_SERIAL=0x3F8

# To run tests during boot
CONFIG_TEST

# Memory manager
MM=flat

# Text-based VGA 80x25 console
CONSOLE_TEXT

# Graphical framebuffer console with custom resolution
#CONSOLE_GRAPHICS
#FB_WIDTH=640
#FB_HEIGHT=480
#FB_BPP=32
```

# Debugging

~/.gdbinit:

```
set print pretty on
set architecture i386:x86-64
target remote :1234
```

Enable following in config/build.cfg:

```
DEBUG     := yes
```

Run 'make boot' and in a different shell start gdb and set a breakpoint to
step through the code:

```
$ gdb src/kernel
Reading symbols from src/kernel...
(gdb) target remote :1234
Remote debugging using :1234
0x0000fff0 in ?? ()
(gdb) break kernel_main
Breakpoint 1 at 0x1007c0: file kernel.c, line 26.
(gdb) c
```

# Reference

* [OSDev Wiki](https://wiki.osdev.org/Main_Page)
