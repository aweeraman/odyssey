# Minos - an experimental operating system for x86

# Build dependencies

```
apt-get install gcc make grub2-common mtools git qemu-system-x86 ovmf
```

Optionally, build using clang and nasm:

```
apt-get install clang nasm
```

Set the following variables in config/build.cfg:

```
CC        := clang
AS        := nasm
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

To boot with coreboot:

```
$ make build-coreboot
$ make boot-coreboot
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
