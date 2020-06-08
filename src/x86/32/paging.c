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

static page_table_entry_t root_page_table[PAGE_TABLE_ENTRIES] \
                          __attribute__((aligned(PAGE_ALIGNMENT)));

void init_paging()
{
        printk("Initializing paging\n");
        printk("  Page directory at 0x%x [%d]\n", &page_dir, PAGE_DIR_ENTRIES);
        printk("  Page table at 0x%x [%d]\n", &root_page_table, PAGE_TABLE_ENTRIES);

        memset(&page_dir, 0, PAGE_DIR_ENTRIES);
        memset(&root_page_table, 0, PAGE_TABLE_ENTRIES);

        /* Initialize the page directory entries */
        for (int i = 0; i < PAGE_DIR_ENTRIES; i++) {
                page_dir[i].present = 0x0;
                page_dir[i].rw      = 0x1;
                page_dir[i].user    = 0x0;
        }

        /* Mapping first 4MB of memory */
        for (int i = 0; i < PAGE_TABLE_ENTRIES; i++) {
                root_page_table[i].present   = 1;
                root_page_table[i].rw        = 1;
                root_page_table[i].user      = 0;
                root_page_table[i].page_addr = (i * 0x1000) >> 12;
        }

        /* Set the mapping for the first page table entry */
        page_dir[0].page_table_addr = ((uint32_t) &root_page_table) >> 12;
        page_dir[0].present         = 1;
        page_dir[0].rw              = 1;
        page_dir[0].user            = 0;

        //init_page_directory((uint32_t *) &page_dir);
        //enable_paging();
}
