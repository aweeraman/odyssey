/**
 * SPDX-FileCopyrightText: 2020 Anuradha Weeraman <anuradha@weeraman.com>
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef FFMM_H
#define FFMM_H

#include <stdint.h>
#include <stddef.h>

#define FRAME_AVAILABLE   0
#define FRAME_INUSE       1
#define FRAME_BLOCK_COUNT 1024
#define FRAME_BLOCK_SIZE  512

struct ff_mm_stats {
        uint32_t bytes_used;
        uint32_t total_bytes;
};

struct ff_mm_frame {
        uint8_t  flags;
        uint32_t addr;
        uint32_t bytes_used;
}__attribute__((packed));

struct ff_mm_superblock {
        uint32_t start_addr;
        uint32_t block_count;
        struct   ff_mm_superblock *next_super_block;
        struct   ff_mm_frame blocks[FRAME_BLOCK_COUNT];
}__attribute__((packed));

typedef struct ff_mm_superblock ff_mm_superblock_t;
typedef struct ff_mm_stats ff_mm_stats_t;

void  init_ff_mm();
void* get_available_frame(ff_mm_superblock_t *sb, size_t size);
void  free_frame(ff_mm_superblock_t *sb, uint32_t *addr);
ff_mm_superblock_t *create_superblock(uint32_t root_block,
                                      uint32_t start_addr,
                                      uint32_t end_addr);
void print_superblocks(ff_mm_superblock_t *sb);
ff_mm_stats_t get_ff_mm_stats(ff_mm_superblock_t *sb, ff_mm_stats_t *stats);

#endif
