#include "kernel.h"
#include "multiboot2.h"
#include "print.h"

void kernel_main(__uint64_t magic, __uint64_t addr) {
  clear();

  // Initialize serial port for communication
  init_serial();
  println("Initialized COM1");

  // Check if bootloader complies with multiboot2
  if (magic == MULTIBOOT2_BOOTLOADER_MAGIC) {
    println("Multiboot2 header valid");
  }

  println("Boot complete, exiting kernel");
}
