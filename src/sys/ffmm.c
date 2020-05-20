/**
 * SPDX-FileCopyrightText: 2020 Anuradha Weeraman <anuradha@weeraman.com>
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include <sys/ffmm.h>
#include <lib/stdio.h>
#include <lib/string.h>

#define MEM_START_ADDR 0x300000 // initialize mm at the 3MB mark
#define MEM_END_ADDR   0x380000

static ff_mm_superblock_t *superblock;

void free_frame(uint32_t *addr)
{
        ff_mm_superblock_t *sb;
        sb = superblock;

        do {
                for (uint32_t i = 0; i < sb->block_count; i++) {
                        if (sb->blocks[i].addr == (uint32_t) addr) {
                                sb->blocks[i].flags = FRAME_AVAILABLE;
                        }
                }

                sb = sb->next_super_block;
        } while (sb != NULL);
}

void* get_available_frame(size_t size)
{
        void* ptr = NULL;
        ff_mm_superblock_t *sb;

        sb = superblock;

        do {
                for (uint32_t i = 0; i < sb->block_count; i++) {
                        if (sb->blocks[i].flags == FRAME_AVAILABLE &&
                                        size < FRAME_BLOCK_SIZE) {
                                sb->blocks[i].flags = FRAME_INUSE;
                                return (void *) sb->blocks[i].addr;
                        }
                }

                sb = sb->next_super_block;
        } while (sb != NULL);

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

        for (uint32_t i = 0; i < sb->block_count; i++) {
                sb->blocks[i].addr = ((uint32_t) sb->start_addr) + (FRAME_BLOCK_SIZE * i);
        }
}

static void print_superblocks(uint32_t root_block)
{
        int count = 0;

        ff_mm_superblock_t *sb;

        sb = (ff_mm_superblock_t *) (uint32_t) root_block;

        do {
                printk("  %d: start: 0x%x, blocks: %d, block_size: %dB\n",
                                count++,
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
        create_superblock(MEM_START_ADDR, 0x400000, 0x480000);
        create_superblock(MEM_START_ADDR, 0x500000, 0x580000);
        print_superblocks(MEM_START_ADDR);
}
