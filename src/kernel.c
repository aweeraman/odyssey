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
 *
 * This program includes code that is Copyright (C) 1999, 2010 Free
 * Software Foundation, Inc.
 */

#include "kernel.h"
#include "libk.h"
#include "boothdr.h"
#include "memory.h"

#ifdef CONFIG_TEST
#include "test.h"
#endif

/*
 * The entry point into the kernel
 */
void kernel_main(size_t magic, size_t addr) {
  init_console();

#ifdef CONFIG_SERIAL
  // Initialize serial port for communication
  init_serial();
  printf("MINOS %s\n", CONFIG_VERSION);
  printf("INITIALIZED SERIAL: %s\n", STRINGIFY(CONFIG_SERIAL));
#else
  printf("MINOS %s\n", CONFIG_VERSION);
#endif

  printf("STACK SIZE: %d\n", CONFIG_STACK);

  init_mb(magic, addr);

#ifdef CONFIG_TEST
  run_tests();
#endif

  while (1) {}

  printf("EXITING KERNEL\n");
}
