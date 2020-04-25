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
#include <stddef.h>

#define MAX_REGION_TYPES  6
#define MAX_REGIONS       100

#define PRINTABLE_CHAR(x) (((x >= 32 && x <= 126) ? x : '.'))

typedef struct basic_meminfo {
  size_t lower;
  size_t upper;
  size_t size;
} basic_meminfo_t;

typedef struct memory_region_struct {
  size_t start;
  size_t end;
  size_t len;
  size_t type;
} memory_region_t;

union mem_ptr {
  // For x86, the order is n4, n3, n2, n1, n8, n7, n6, n5
  struct {
    uint8_t n1: 4;
    uint8_t n2: 4;
    uint8_t n3: 4;
    uint8_t n4: 4;
    uint8_t n5: 4;
    uint8_t n6: 4;
    uint8_t n7: 4;
    uint8_t n8: 4;
  } nibble;
  // For x86, the order is b2, b1, b4, b3
  struct {
    uint8_t b1;
    uint8_t b2;
    uint8_t b3;
    uint8_t b4;
  } byte;
  // For x86, the order is w2, w1
  struct {
    uint16_t w1;
    uint16_t w2;
  } word;
} __attribute__((packed));
typedef union mem_ptr mem_ptr_t;

void set_basic_meminfo(size_t lower, size_t upper);
void add_mem_region(int idx, size_t start, size_t len, size_t type);
memory_region_t get_mem_region(int idx);
void set_num_mem_regions(int num);
void print_mem_regions();
void peek(size_t addr, int count);

#endif
