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

static mm_superblock_t *superblock;

mm_stats_t get_mm_stats(mm_superblock_t *sb, mm_stats_t *stats)
{
        if (sb == NULL)
                sb = superblock;

        stats->bytes_used  = 0;
        stats->total_bytes = 0;

        do {
                for (uint32_t i = 0; i < sb->block_count; i++) {
                        if (sb->blocks[i].flags == FRAME_INUSE ||
                            sb->blocks[i].flags == FRAME_MULTI_START ||
                            sb->blocks[i].flags == FRAME_MULTI) {
                                stats->bytes_used += sb->blocks[i].bytes_used;
                        }
                        stats->total_bytes += FRAME_BLOCK_SIZE;
                }

                sb = sb->next_super_block;
        } while (sb != NULL);

        return (mm_stats_t) (*stats);
}

void free_frame(mm_superblock_t *sb, uint32_t *addr)
{
        if (sb == NULL)
                sb = superblock;

        do {
                int multi_start = 0;
                for (uint32_t i = 0; i < sb->block_count; i++) {
                        if ((uint32_t) sb->blocks[i].addr == (uint32_t) addr &&
                            (sb->blocks[i].flags == FRAME_MULTI_START)) {

                                multi_start = 1;
                                sb->blocks[i].flags      = FRAME_AVAILABLE;
                                sb->blocks[i].bytes_used = 0;
                                memset(sb->blocks[i].addr, '\0', FRAME_BLOCK_SIZE);

                        } else if ((uint32_t) sb->blocks[i].addr == (uint32_t) addr &&
                                  (sb->blocks[i].flags == FRAME_INUSE)) {

                                sb->blocks[i].flags      = FRAME_AVAILABLE;
                                sb->blocks[i].bytes_used = 0;
                                memset(sb->blocks[i].addr, '\0', FRAME_BLOCK_SIZE);

                        } else if (multi_start == 1 && (sb->blocks[i].flags == FRAME_MULTI)) {

                                sb->blocks[i].flags      = FRAME_AVAILABLE;
                                sb->blocks[i].bytes_used = 0;
                                memset(sb->blocks[i].addr, '\0', FRAME_BLOCK_SIZE);

                        } else if (multi_start == 1 &&
                                  ((sb->blocks[i].flags == FRAME_MULTI_START)
                                    || (sb->blocks[i].flags == FRAME_INUSE))) {
                                multi_start = 0;
                        }
                }

                sb = sb->next_super_block;
        } while (sb != NULL);
}

void* get_available_frame(mm_superblock_t *sb, size_t size)
{
        void* ptr = NULL;

        if (sb == NULL)
                sb = superblock;

        do {
                if (size < FRAME_BLOCK_SIZE) {
                        for (uint32_t i = 0; i < sb->block_count; i++) {
                                if (sb->blocks[i].flags == FRAME_AVAILABLE) {
                                        sb->blocks[i].flags      = FRAME_INUSE;
                                        sb->blocks[i].bytes_used = size;
                                        memset(sb->blocks[i].addr, '\0', FRAME_BLOCK_SIZE);
                                        return (void *) sb->blocks[i].addr;
                                }
                        }
                } else {
                        int start_block       = -1;
                        int last_block        = -1;
                        int contiguous_blocks = 0;
                        int size_remaining    = size;

                        for (uint32_t i = 0; i < sb->block_count; i++) {
                                if (sb->blocks[i].flags == FRAME_AVAILABLE) {
                                        if (start_block == -1) {
                                                start_block     = i;
                                                size_remaining -= FRAME_BLOCK_SIZE;
                                                last_block      = i;
                                                contiguous_blocks++;
                                        } else {
                                                if (last_block == (int) (i - 1)) {
                                                        size_remaining -= FRAME_BLOCK_SIZE;
                                                        last_block      = i;
                                                        contiguous_blocks++;

                                                        if (size_remaining <= 0)
                                                                break;
                                                } else {
                                                        start_block       = -1;
                                                        last_block        = -1;
                                                        contiguous_blocks = 0;
                                                        size_remaining    = size;
                                                }
                                        }
                                }
                        }

                        if (size_remaining > 0)
                                return NULL;

                        if (start_block != -1) {
                                size_remaining = size;
                                for (int i = start_block; i < contiguous_blocks; i++) {
                                        if (sb->blocks[i].flags == FRAME_AVAILABLE) {
                                                if (i == start_block)
                                                        sb->blocks[i].flags = FRAME_MULTI_START;
                                                else
                                                        sb->blocks[i].flags = FRAME_MULTI;
                                                if (size_remaining > FRAME_BLOCK_SIZE) {
                                                        sb->blocks[i].bytes_used = FRAME_BLOCK_SIZE;
                                                } else {
                                                        sb->blocks[i].bytes_used = size_remaining;
                                                }
                                                size_remaining -= FRAME_BLOCK_SIZE;
                                                memset(sb->blocks[i].addr, '\0', FRAME_BLOCK_SIZE);
                                        }
                                }
                                return (void *) sb->blocks[start_block].addr;
                        }
                }

                sb = sb->next_super_block;
        } while (sb != NULL);

        return ptr;
}

mm_superblock_t *create_superblock(uint32_t root_block,
                                   uint32_t start_addr,
                                   uint32_t end_addr)
{
        uint32_t available_memory;
        uint32_t blocks;
        mm_superblock_t *sb;

        sb = (mm_superblock_t *) (uint32_t) root_block;

        if (root_block == start_addr) {
                memset(sb, '\0', sizeof(mm_superblock_t));
        } else {
                while (sb->next_super_block != NULL)
                       sb = sb->next_super_block;

                sb->next_super_block = (mm_superblock_t *) (uint32_t) start_addr;
                sb = sb->next_super_block;
                memset(sb, '\0', sizeof(mm_superblock_t));
        }

        sb->start_addr       = start_addr + sizeof(mm_superblock_t) + 1;
        sb->next_super_block = NULL;

        available_memory = end_addr - start_addr;
        blocks = (uint32_t) available_memory / FRAME_BLOCK_SIZE;

        sb->block_count = blocks;

        for (uint32_t i = 0; i < sb->block_count; i++) {
                sb->blocks[i].addr = (uint32_t *) ((uint32_t) sb->start_addr)
                                                  + (FRAME_BLOCK_SIZE * i);
                sb->blocks[i].bytes_used = 0;
                sb->blocks[i].flags = FRAME_AVAILABLE;
        }

        return sb;
}

void print_superblocks(mm_superblock_t *sb)
{
        int count = 0;

        if (sb == NULL)
                sb = superblock;

        do {
                printk("  %d: start: 0x%x, blocks: %d, block_size: %dB\n",
                                count++,
                                sb->start_addr,
                                sb->block_count,
                                FRAME_BLOCK_SIZE);
                sb = sb->next_super_block;
        } while (sb != NULL);
}

void init_mm()
{
        printk("Initializing FirstFit memory manager at 0x%x\n", MEM_START_ADDR);
#ifdef ARCH_X86
        superblock = create_superblock(MEM_START_ADDR,
                                       MEM_START_ADDR,
                                       MEM_END_ADDR);
#elif ARCH_ARM
        superblock = create_superblock(MEM_START_ADDR,
                                       MEM_START_ADDR,
                                       MEM_END_ADDR);
#endif
        print_superblocks(NULL);
}
