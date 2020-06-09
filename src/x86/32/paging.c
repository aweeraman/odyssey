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

static page_dir_entry_t kernel_page_dir __attribute__((aligned(PAGE_ALIGNMENT)));

static void map_memory()
{
        uint32_t current_phys_addr = 0x0;
        uint32_t start_phys_addr   = 0x0;
        uint32_t end_phys_addr     = 0x450000;

        while (current_phys_addr < end_phys_addr) {
                uint32_t page_table_idx = current_phys_addr >> 22;
                uint32_t page_idx = (current_phys_addr >> 12) & 0x3ff;

                kernel_page_dir.tables[page_idx].addr      = current_phys_addr >> 12;
                kernel_page_dir.tables[page_idx].present   = 1;
                kernel_page_dir.tables[page_idx].rw        = 1;
                kernel_page_dir.tables[page_idx].user      = 0;

                current_phys_addr += 0x1000;

                // TODO clean this up
                kernel_page_dir.directory[page_table_idx].addr      = (uint32_t) kernel_page_dir.tables >> 12;
                kernel_page_dir.directory[page_table_idx].present   = 1;
                kernel_page_dir.directory[page_table_idx].rw        = 1;
                kernel_page_dir.directory[page_table_idx].user      = 0;
        }

        peek(&kernel_page_dir.directory, 1);
        peek(&kernel_page_dir.tables[0x234], 1);
}

void init_paging()
{
        printk("Initializing paging\n");
        printk("  Page directory at 0x%x\n", &kernel_page_dir);
        memset(&kernel_page_dir, 0, sizeof(page_dir_entry_t));
        map_memory();
        switch_page_directory((uint32_t *) kernel_page_dir.directory);
        enable_paging();
}
