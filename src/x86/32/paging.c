/**
 * SPDX-FileCopyrightText: 2020 Anuradha Weeraman <anuradha@weeraman.com>
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include <x86/32/paging.h>
#include <lib/string.h>
#include <lib/stdio.h>

#define PAGE_ALIGNMENT     4096
#define PAGE_DIR_ENTRIES   1024
#define PAGE_TABLE_ENTRIES 1024

static page_dir_entry_t kernel_pg_dir __attribute__((aligned(PAGE_ALIGNMENT)));

static void map_memory(uint32_t phys_start, uint32_t phys_end)
{
        uint32_t phys_cur = phys_start;

        while (phys_cur < phys_end) {
                uint32_t table_idx = phys_cur >> 22;
                uint32_t page_idx = (phys_cur >> 12) & 0x3ff;

                kernel_pg_dir.tables[page_idx].addr      = phys_cur >> 12;
                kernel_pg_dir.tables[page_idx].present   = 1;
                kernel_pg_dir.tables[page_idx].rw        = 1;
                kernel_pg_dir.tables[page_idx].user      = 0;

                phys_cur += 0x1000;

                // TODO clean this up
                kernel_pg_dir.directory[table_idx].addr      = (uint32_t) kernel_pg_dir.tables >> 12;
                kernel_pg_dir.directory[table_idx].present   = 1;
                kernel_pg_dir.directory[table_idx].rw        = 1;
                kernel_pg_dir.directory[table_idx].user      = 0;
        }

        peek(&kernel_pg_dir.directory, 1);
        peek(&kernel_pg_dir.tables[0x234], 1);
}

void init_paging()
{
        printk("Initializing paging, kernel page directory at 0x%x\n", &kernel_pg_dir);
        memset(&kernel_pg_dir, 0, sizeof(page_dir_entry_t));
        map_memory(0x0, 0x450000);
        switch_page_directory((uint32_t *) kernel_pg_dir.directory);
        enable_paging();
}
