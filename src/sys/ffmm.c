/**
 * SPDX-FileCopyrightText: 2020 Anuradha Weeraman <anuradha@weeraman.com>
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include <sys/ffmm.h>
#include <lib/stdio.h>
#include <lib/string.h>

#define MEM_START_ADDR 0x300000 // initialize mm at the 3MB mark
#define MEM_FRAME_SIZE 512      // size of each frame of memory
#define MEM_END_ADDR   0x400000

static ff_mm_superblock_t *superblock;

void* get_free_frame(size_t size)
{
        void* ptr = NULL;

        return ptr;
}

static void create_superblock(uint32_t root_block, uint32_t start_addr, uint32_t end_addr)
{
        uint32_t available_memory;
        uint32_t blocks;
        ff_mm_superblock_t *sb;

        sb = (ff_mm_superblock_t *) (uint32_t) root_block;

        if (root_block == start_addr) {
                memset(sb, '\0', sizeof(ff_mm_superblock_t));
                superblock = sb;
        } else {
                while (sb->next_super_block != NULL)
                        sb = sb->next_super_block;

                sb->next_super_block = (ff_mm_superblock_t *) (uint32_t) start_addr;
                sb = (ff_mm_superblock_t *) sb->next_super_block;
        }

        sb->magic            = FF_MAGIC;
        sb->start_addr       = start_addr + sizeof(ff_mm_superblock_t) + 1;
        sb->next_super_block = NULL;

        available_memory = end_addr - start_addr;
        blocks = available_memory / FRAME_BLOCK_SIZE;

        sb->block_count      = blocks;
}

static void print_superblocks(uint32_t root_block)
{
        ff_mm_superblock_t *sb;

        sb = (ff_mm_superblock_t *) (uint32_t) root_block;

        do {
                printk("  start: 0x%x, blocks: %d, block_size: %dB\n",
                                sb->start_addr,
                                sb->block_count,
                                FRAME_BLOCK_SIZE);
                sb = sb->next_super_block;
        } while (sb != NULL);


}

void init_ff_mm()
{
        printk("Initializing FirstFit memory manager at 0x%x\n", MEM_START_ADDR);
        create_superblock(MEM_START_ADDR, MEM_START_ADDR, MEM_END_ADDR);
        create_superblock(MEM_START_ADDR, 0x500000, 0x550000);
        print_superblocks(MEM_START_ADDR);
}
