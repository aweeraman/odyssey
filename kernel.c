#include "multiboot2.h"
#include "print.h"

void kernel_main(unsigned long magic, unsigned long addr) {
  clear_screen();
  init_serial();
  print("Initialized COM1\n");
  print("Boot complete\n");
}
