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

struct ff_mm_frame {
        uint8_t  flags;
        uint32_t addr;
}__attribute__((packed));

struct ff_mm_superblock {
        uint32_t start_addr;
        uint32_t block_count;
        struct   ff_mm_superblock *next_super_block;
        struct   ff_mm_frame blocks[FRAME_BLOCK_COUNT];
}__attribute__((packed));

typedef struct ff_mm_superblock ff_mm_superblock_t;

void  init_ff_mm();
void* get_available_frame(size_t size);
void  free_frame(uint32_t *addr);

#endif
