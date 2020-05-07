[![Coverity Scan Build Status](https://scan.coverity.com/projects/20852/badge.svg)](https://scan.coverity.com/projects/minos)

# Minos - an experimental operating system for x86

![Minos running in Qemu](https://raw.githubusercontent.com/aweeraman/minos/master/minos.png "Minos running in Qemu")

# Current Development Status

- [X] Multiboot2
- [X] EGA 80x25 console
- [X] Scalable console fonts
- [X] Serial output
- [X] RGB framebuffer
- [X] Build-time customization of features
- [X] Segmentation
- [X] Interrupt handling
- [X] Keyboard support
- [ ] Timers
- [ ] Memory manager
- [ ] Paging
- [ ] User-mode
- [ ] System calls
- [ ] File system
- [ ] Multitasking
- [ ] Process scheduler
- [ ] Standard C-library
- [ ] Shell and basic UNIX commands

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
$ sudo apt-get install grub-common mtools qemu-system-gui qemu-system-x86 ovmf exuberant-ctags
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

# Memory manager
# options: ff=FirstFit
CONFIG_MM=ff

# Framebuffer console
CONFIG_FRAMEBUFFER_RGB
CONFIG_FRAMEBUFFER_WIDTH=1024
CONFIG_FRAMEBUFFER_HEIGHT=768
CONFIG_FRAMEBUFFER_BPP=32

# PS/2 keyboard
CONFIG_DRV_KEYBOARD

# Run tests during boot
CONFIG_TEST
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

# Attribution

Minos makes use of following software:

* [Coreboot](https://github.com/coreboot/coreboot) - GPL
* [Scalable Screen Font](https://gitlab.com/bztsrc/scalable-font) - MIT License
* [GNU unifont](http://unifoundry.com/unifont/index.html) - GPL

# Reference

* [OSDev Wiki](https://wiki.osdev.org/Main_Page) is an invaluable resource
for hobbyist operating systems development.
* [Intel® 64 and IA-32 Architectures Software Developer Manuals](https://software.intel.com/en-us/articles/intel-sdm)

# License

Minos is distributed under the GPLv3 license.
