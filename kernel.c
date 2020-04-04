#include "kernel.h"
#include "multiboot2.h"
#include "print.h"

void kernel_main(__uint64_t magic, __uint64_t addr) {
  clear_screen();

  // Initialize serial port for communication
  init_serial();
  print("Initialized COM1\n");

  // Check if bootloader complies with multiboot2
  if (magic == MULTIBOOT2_BOOTLOADER_MAGIC) {
    print("Multiboot2 header valid\n");
  }

  print("Boot complete, exiting kernel\n");
}
