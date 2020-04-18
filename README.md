[![Coverity Scan Build Status](https://scan.coverity.com/projects/20852/badge.svg)](https://scan.coverity.com/projects/minos)

# Minos - an experimental operating system for x86

# Build dependencies

Build GCC cross compiler and openboot:

```
$ sudo apt-get install git gcc g++ make m4 bison flex bzip2 xz-utils curl zlib1g-dev gnat python
$ git clone https://github.com/aweeraman/minos.git
$ cd minos
$ make deps
```

Install additional dependencies required for building and running minos
in qemu:

```
apt-get install grub-common mtools qemu-system-x86 ovmf
```

Optionally, if you wish to use clang, you can install it and configure
it in the build config as follows:

```
apt-get install clang
```

Set the following variables in config/build.cfg to use clang:

```
CC        := clang
```

nasm is available as part of the cross compiler dependencies and can
be used in place GNU as for building the assembly programs by setting
the following in in config/build.cfg.  The GNU assembler will be used
by default.

```
AS        := ../coreboot/util/crossgcc/xgcc/bin/nasm
```

# Running in qemu

```
$ make
$ make boot
```

To boot in qemu with EFI:

```
$ make boot-efi
```

# Configuration

List capabilities to be included in the kernel in config/kernel.cfg.
Currently available options are:

```
CONFIG_VERSION_MAJOR=0
CONFIG_VERSION_MINOR=1

CONFIG_STACK=4096

# COM1=0x3F8,COM2=0x2F8,COM3=0x3E8,COM4=0x2E8
CONFIG_SERIAL=0x3F8
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
