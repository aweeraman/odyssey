# Minos - an experimental operating system for x86

# Build dependencies

```
apt-get install gcc make grub2-common mtools git qemu-system-x86 ovmf
```

Optionally, build using clang and nasm:

```
apt-get install clang nasm
```

Set the following variables in the Makefile:

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
