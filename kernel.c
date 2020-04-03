#include "multiboot2.h"
#include "print.h"

void kernel_main(unsigned long magic, unsigned long addr) {
  clear_screen();
  print("booted");
}
