# An experiment in operating system dev

## Building coreboot
```
$ git clone git@github.com:coreboot/coreboot.git
$ time make crossgcc-i386 CPUS=12
$ time make -j12
```

## Running qemu with coreboot/SeaBIOS
```
$ qemu-system-i386 -bios coreboot/build/coreboot.rom -cdrom image.iso
```
