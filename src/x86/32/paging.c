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

static page_dir_entry_t page_dir __attribute__((aligned(PAGE_ALIGNMENT)));

void init_paging()
{
        uint32_t current_phys_addr = 0x0;
        uint32_t start_phys_addr   = 0x0;
        uint32_t end_phys_addr     = 0x450000;

        printk("Initializing paging\n");
        printk("  Page directory at 0x%x\n", &page_dir);

        memset(&page_dir, 0, sizeof(page_dir_entry_t));

        while (current_phys_addr < end_phys_addr) {
                uint32_t page_table_idx = current_phys_addr >> 22;
                uint32_t page_idx = (current_phys_addr >> 12) & 0x3ff;

                page_dir.tables[page_idx].addr      = current_phys_addr >> 12;
                page_dir.tables[page_idx].present   = 1;
                page_dir.tables[page_idx].rw        = 1;
                page_dir.tables[page_idx].user      = 0;

                current_phys_addr += 0x1000;

                // TODO clean this up
                page_dir.directory[page_table_idx].addr      = (uint32_t) page_dir.tables >> 12;
                page_dir.directory[page_table_idx].present   = 1;
                page_dir.directory[page_table_idx].rw        = 1;
                page_dir.directory[page_table_idx].user      = 0;
        }

        //peek(&page_dir.directory, 1);
        //peek(&page_dir.tables[0x234], 1);

        //init_page_directory((uint32_t *) page_dir.directory);
        //enable_paging();
}
