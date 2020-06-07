/**
 * SPDX-FileCopyrightText: 2020 Anuradha Weeraman <anuradha@weeraman.com>
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include <mm/region.h>
#include <sys/tty.h>
#include <lib/stdio.h>
#include <sys/panic.h>
#include <lib/mm.h>

static char *MEMORY_REGION_T[MAX_REGION_TYPES] = {
  "other",
  "available",
  "reserved",
  "acpi",
  "acpi nvs",
  "bad"
};

static basic_meminfo_t mem_info;
static memory_region_t *mem_regions;
static int num_regions = 0;
static int max_regions = 0;

void init_mem_regions(size_t count)
{
        mem_regions = kzalloc(NULL, sizeof(memory_region_t), count);
        max_regions = count;
}

void set_basic_meminfo(size_t lower, size_t upper)
{
        mem_info.lower = lower;
        mem_info.upper = upper;
}

/*
 * Extract the type of memory region
 */
static char *mem_reg_lookup(int type)
{
        if (type >= MAX_REGION_TYPES)
                return MEMORY_REGION_T[0];

        return MEMORY_REGION_T[type];
}

void print_mem_regions()
{
        printk("Memory map\n");
        for (int idx = 0; idx < num_regions; idx++) {
                memory_region_t mem_reg = mem_regions[idx];
                printk("  %d: 0x%x - 0x%x %d (%s)\n",
                        idx,
                        mem_reg.start,
                        mem_reg.end,
                        mem_reg.len,
                        mem_reg_lookup(mem_reg.type));
        }
}

void add_mem_region(size_t start, size_t len, size_t type)
{
        if (num_regions >= max_regions)
                panic("Exceeded maximum number of memory regions");

        mem_regions[num_regions].start = start;
        mem_regions[num_regions].end   = start+len-1;
        mem_regions[num_regions].len   = len-1;
        mem_regions[num_regions].type  = type;

        num_regions++;
}
