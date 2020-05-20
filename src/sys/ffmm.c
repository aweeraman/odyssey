/**
 * SPDX-FileCopyrightText: 2020 Anuradha Weeraman <anuradha@weeraman.com>
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include <sys/ffmm.h>
#include <lib/stdio.h>
#include <lib/string.h>

#ifdef ARCH_X86
#define MEM_START_ADDR 0x300000
#define MEM_END_ADDR   0x380000
#elif ARCH_ARM
#define MEM_START_ADDR 0x90300000
#define MEM_END_ADDR   0x90380000
#endif

static ff_mm_superblock_t *superblock;

void free_frame(ff_mm_superblock_t *sb, uint32_t *addr)
{
        do {
                for (uint32_t i = 0; i < sb->block_count; i++) {
                        if (sb->blocks[i].addr == (uint32_t) addr) {
                                sb->blocks[i].flags = FRAME_AVAILABLE;
                        }
                }

                sb = sb->next_super_block;
        } while (sb != NULL);
}

void* get_available_frame(ff_mm_superblock_t *sb, size_t size)
{
        void* ptr = NULL;

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

ff_mm_superblock_t *create_superblock(uint32_t root_block,
                                      uint32_t start_addr,
                                      uint32_t end_addr)
{
        uint32_t available_memory;
        uint32_t blocks;
        ff_mm_superblock_t *sb;

        sb = (ff_mm_superblock_t *) (uint32_t) root_block;

        if (root_block == start_addr) {
                memset(sb, '\0', sizeof(ff_mm_superblock_t));
        } else {
                while (sb->next_super_block != NULL)
                       sb = sb->next_super_block;

                sb->next_super_block = (ff_mm_superblock_t *) (uint32_t) start_addr;
                sb = sb->next_super_block;
        }

        sb->start_addr       = start_addr + sizeof(ff_mm_superblock_t) + 1;
        sb->next_super_block = NULL;

        available_memory = end_addr - start_addr;
        blocks = (uint32_t) available_memory / FRAME_BLOCK_SIZE;

        sb->block_count = blocks;

        for (uint32_t i = 0; i < sb->block_count; i++) {
                sb->blocks[i].addr = ((uint32_t) sb->start_addr) + (FRAME_BLOCK_SIZE * i);
        }

        return sb;
}

void print_superblocks(ff_mm_superblock_t *sb)
{
        int count = 0;

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
#ifdef ARCH_X86
        superblock = create_superblock(MEM_START_ADDR,
                                       MEM_START_ADDR,
                                       MEM_END_ADDR);
        create_superblock((uint32_t) superblock, 0x400000, 0x480000);
        create_superblock((uint32_t) superblock, 0x500000, 0x580000);
#elif ARCH_ARM
        superblock = create_superblock(MEM_START_ADDR,
                                       MEM_START_ADDR,
                                       MEM_END_ADDR);
        create_superblock((uint32_t) superblock, 0x90400000, 0x90480000);
        create_superblock((uint32_t) superblock, 0x90500000, 0x90580000);
#endif
        print_superblocks(superblock);
}
