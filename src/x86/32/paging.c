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

static page_dir_entry_t   page_dir[PAGE_DIR_ENTRIES] \
                          __attribute__((aligned(PAGE_ALIGNMENT)));

static uint32_t pagedir[PAGE_DIR_ENTRIES] \
                          __attribute__((aligned(PAGE_ALIGNMENT)));

static page_table_entry_t root_page_table[PAGE_TABLE_ENTRIES] \
                          __attribute__((aligned(PAGE_ALIGNMENT)));

void init_paging()
{
        uint32_t current_phys_addr = 0x0;
        uint32_t start_phys_addr   = 0x0;
        uint32_t end_phys_addr     = 0x400000;

        printk("Initializing paging\n");
        printk("  Page directory at 0x%x [%d]\n", &page_dir, PAGE_DIR_ENTRIES);
        printk("  Page table at 0x%x [%d]\n", &root_page_table, PAGE_TABLE_ENTRIES);

        memset(&page_dir, 0, PAGE_DIR_ENTRIES);
        memset(&root_page_table, 0, PAGE_TABLE_ENTRIES);

        while (current_phys_addr < end_phys_addr) {
                uint32_t page_table_idx = current_phys_addr >> 22;
                uint32_t page_idx = (current_phys_addr >> 12) & 0x3ff;

                root_page_table[page_idx].page_addr = current_phys_addr / 0x1000;
                root_page_table[page_idx].present   = 1;
                root_page_table[page_idx].rw        = 1;
                root_page_table[page_idx].user      = 0;

                current_phys_addr += 0x1000;

                page_dir[page_table_idx].page_table_addr = (uint32_t) root_page_table >> 4;
                page_dir[page_table_idx].present         = 1;
                page_dir[page_table_idx].rw              = 1;
                page_dir[page_table_idx].user            = 0;
        }

        peek(&page_dir, 1);
        peek(&root_page_table[0x3ff], 1);

        init_page_directory((uint32_t *) page_dir);
        enable_paging();
}
