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

#include "memory.h"
#include "tty.h"
#include "libk.h"

static char *MEMORY_REGION_T[MAX_REGION_TYPES] = {
  "UNKNOWN",
  "AVAILABLE",
  "RESERVED",
  "ACPI",
  "ACPI NVS",
  "BAD"
};

static memory_region_t mem_regions[MAX_REGIONS];
static int num_regions = 0;

void peek(uint32_t addr, int count) {
  mem_ptr_t *p = (mem_ptr_t *) addr;
#ifdef CONFIG_ARCH_X86_32
  for (int i = 0; i < count; i++) {
    printk("Peek addr=%x, %x%x%x%x %x%x%x%x\n", p, p->nibble.n4,
        p->nibble.n3, p->nibble.n2, p->nibble.n1, p->nibble.n8,
        p->nibble.n7, p->nibble.n6, p->nibble.n5);
    p += 1;
  }
#endif
}

/*
 * Extract the type of memory region
 */
static char *mem_reg_lookup(int type) {
  if (type >= MAX_REGION_TYPES) {
    return MEMORY_REGION_T[0];
  }

  return MEMORY_REGION_T[type];
}

void print_mem_regions() {
  printk("MEMORY MAP\n");
  for (int idx = 0; idx <= num_regions; idx++) {
    memory_region_t mem_reg = get_mem_region(idx);
    printk("  %d: 0x%x - 0x%x %d [%s]\n",
        idx,
        mem_reg.start,
        mem_reg.end,
        mem_reg.len,
        mem_reg_lookup(mem_reg.type));
  }
}

void set_num_mem_regions(int num) {
  num_regions = num;
}

void add_mem_region(int idx, uint32_t start, uint32_t len, uint32_t type) {
  // TODO: deprecate this with dynamic memory allocation
  if (idx >= MAX_REGIONS) {
    panic("exceeded maximum number of memory regions");
  }

  mem_regions[idx].start = start;
  mem_regions[idx].end   = start+len-1;
  mem_regions[idx].len   = len-1;
  mem_regions[idx].type  = type;
}

memory_region_t get_mem_region(int idx) {
  return mem_regions[idx];
}
