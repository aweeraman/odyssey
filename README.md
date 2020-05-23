[![Coverity Scan Build Status](https://scan.coverity.com/projects/20852/badge.svg)](https://scan.coverity.com/projects/minos)

# Odyssey - an experimental operating system for x86 and ARM

![Odyssey running in Qemu](https://raw.githubusercontent.com/aweeraman/odyssey/master/docs/img/odyssey.png "Odyssey running in Qemu")

# Current Development Status

## Architecture: x86 (i386)
- [X] Multiboot2
- [X] Protected mode
- [X] EGA 80x25 console
- [X] Scalable console fonts
- [X] Serial output
- [X] RGB framebuffer
- [X] Build-time customization of features
- [X] Segmentation
- [X] Interrupt handling
- [X] Timers
- [X] PS/2 keyboard support
- [X] Memory manager
- [ ] Paging
- [ ] User-mode
- [ ] System calls
- [ ] File system
- [ ] Multitasking
- [ ] Process scheduler
- [ ] Standard C-library
- [ ] Shell and basic UNIX commands

## Architecture: ARM (armv7-a)
- [ ] EGA 80x25 console
- [X] Serial output
- [ ] RGB framebuffer
- [X] Build-time customization of features
- [ ] Segmentation
- [ ] Interrupt handling
- [ ] Timers
- [ ] PS/2 keyboard support
- [X] Memory manager
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
$ git clone https://github.com/aweeraman/odyssey.git
$ sudo apt-get install git gcc g++ make m4 bison flex bzip2 xz-utils curl zlib1g-dev gnat python
```

Build GCC cross compilers for i386, armv7-a and boot loaders:

```
$ cd odyssey
$ make deps
```

Install additional dependencies required for building and running odyssey
in qemu:

```
$ sudo apt-get install grub-common mtools qemu-system-gui qemu-system-x86 qemu-system-arm ovmf exuberant-ctags u-boot-tools
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

To build and run in qemu:

```
$ make boot
```

or, to build the ARM version:

```
$ make ARCH=arm boot
```

To build and run in qemu with Coreboot:

```
$ make boot-coreboot
```

To build and run in qemu with OVMS/EFI:

```
$ make boot-efi
```

# Running in qemu on ARM/U-Boot

Install a tftp-server to serve the OS image:

```
$ sudo apt-get install tftpd-hpa lrzsz
$ sudo systemctl start tftpd-hpa
```

Build the OS image and copy over to the tftp server location:

```
$ make ARCH=arm
$ make ARCH=arm img
$ sudo cp odyssey.img /srv/tftp
$ make ARCH=arm boot-uboot
```

At the U-Boot prompt, enter the following:

```
setenv ipaddr 10.x.x.x
setenv serverip 192.x.x.x
tftp 80000000 odyssey.img
iminfo 80000000
bootm 80000000
```

# Configuration

List capabilities to be included in the kernel in config/kernel.cfg.
Currently available options are:

```
# Prefix with a leading hash (#) to disable a feature

CONFIG_VERSION_MAJOR=0
CONFIG_VERSION_MINOR=1

# Drivers / hardware support
CONFIG_SERIAL
CONFIG_KEYBOARD

# Framebuffer console
CONFIG_FRAMEBUFFER_RGB
CONFIG_FRAMEBUFFER_WIDTH=1024
CONFIG_FRAMEBUFFER_HEIGHT=768
CONFIG_FRAMEBUFFER_BPP=32

# FirstFit memory manager
CONFIG_MM_FF

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
$ gdb src/odyssey
Reading symbols from src/odyssey...
(gdb) target remote :1234
Remote debugging using :1234
0x0000fff0 in ?? ()
(gdb) break kernel_main
Breakpoint 1 at 0x1007c0: file main.c, line 26.
(gdb) c
```

# Testing

On BeagleBone black, use loady at the U-Boot prompt to load the image over serial/ymodem:

```
$ make ARCH=arm
$ screen /dev/ttyUSB0 115200
[power-on the board]
[press space to interrupt auto-boot]
=> loady
<CTRL-a>:exec !! sx --ymodem odyssey.img
=> go 0x82000000
```

# Use of Free and Open Source Software

Odyssey makes use of following free and open source software:

* [GCC](https://gcc.gnu.org/)
* [Clang](https://clang.llvm.org/)
* [NASM](https://www.nasm.us/)
* [GNU Make](https://www.gnu.org/software/make/)
* [QEMU](https://www.qemu.org/)
* [GNU GRUB](https://www.gnu.org/software/grub/)
* [Coreboot](https://github.com/coreboot/coreboot)
* [U-Boot](https://github.com/u-boot/u-boot)
* [OVMF](https://github.com/tianocore/tianocore.github.io/wiki/OVMF)
* [Scalable Screen Font](https://gitlab.com/bztsrc/scalable-font)
* [CTAGS](http://ctags.sourceforge.net/)

# License

Odyssey is distributed under the GPLv3 license.

# Reference

* [OSDev Wiki](https://wiki.osdev.org/Main_Page)
* [/r/osdev](https://www.reddit.com/r/osdev)
* [alt.os.development](https://groups.google.com/forum/#!forum/alt.os.development)
* [Intel® 64 and IA-32 Architectures Software Developer Manuals](https://software.intel.com/en-us/articles/intel-sdm)
* [CoreTile Express A-15 Technical Reference Manual](http://infocenter.arm.com/help/index.jsp?topic=/com.arm.doc.subset.boards.express/index.html)
* [TI AM3358 TRM](https://www.ti.com/product/AM3358)
