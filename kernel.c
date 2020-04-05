/**
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 * Copyright 2020, Anuradha Weeraman
 */

#include "kernel.h"
#include "multiboot2.h"
#include "print.h"
#include "strings.h"

void kernel_main(__uint64_t magic, __uint64_t addr) {
  clear();

  // Initialize serial port for communication
  init_serial();
  println("Initialized COM1");

  // Check if bootloader complies with multiboot2
  if (magic == MULTIBOOT2_BOOTLOADER_MAGIC) {
    printf("Multiboot2 header 0x%x valid\n", magic);
  }

  printf("Multiboot2 information structure size: %d bytes\n", *(size_t *) addr);

  println("Boot complete, exiting kernel");
}
