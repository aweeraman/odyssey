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
    printk("  %d: 0x%u - 0x%u %d [%s]\n",
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

void add_mem_region(int idx, uint64_t start, uint64_t end, uint32_t len, uint32_t type) {
  // TODO: deprecate this with dynamic memory allocation
  if (idx >= MAX_REGIONS) {
    panic("exceeded maximum number of memory regions");
  }

  mem_regions[idx].start = start;
  mem_regions[idx].end   = end;
  mem_regions[idx].len   = len;
  mem_regions[idx].type  = type;
}

memory_region_t get_mem_region(int idx) {
  return mem_regions[idx];
}
