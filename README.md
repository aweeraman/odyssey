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
