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

#ifndef MEMORY_H
#define MEMORY_H

#include <stdint.h>

#define MAX_REGION_TYPES  6
#define MAX_REGIONS       100

typedef struct memory_region_struct {
  uint64_t start;
  uint64_t end;
  uint32_t len;
  uint32_t type;
} memory_region_t;

void add_mem_region(int idx, uint64_t start, uint32_t len, uint32_t type);
memory_region_t get_mem_region(int idx);
void set_num_mem_regions(int num);
void print_mem_regions();

#endif
