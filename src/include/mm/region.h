/**
 * SPDX-FileCopyrightText: 2020 Anuradha Weeraman <anuradha@weeraman.com>
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef REGION_H
#define REGION_H

#include <stdint.h>
#include <stddef.h>

#define MAX_REGION_TYPES  6
#define MAX_REGIONS       100

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

void set_basic_meminfo(size_t lower, size_t upper);
void add_mem_region(size_t start, size_t len, size_t type);
void print_mem_regions();

#endif
