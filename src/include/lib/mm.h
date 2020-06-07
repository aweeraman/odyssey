/**
 * SPDX-FileCopyrightText: 2020 Anuradha Weeraman <anuradha@weeraman.com>
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef MM_H
#define MM_H

#include <stddef.h>
#include <mm/ff.h>

#define PRINTABLE_CHAR(x) (((x >= 32 && x <= 126) ? x : '.'))

union mem_ptr {
        struct {
                uint8_t n1: 4;
                uint8_t n2: 4;
                uint8_t n3: 4;
                uint8_t n4: 4;
                uint8_t n5: 4;
                uint8_t n6: 4;
                uint8_t n7: 4;
                uint8_t n8: 4;
        } nibble;
        struct {
                uint8_t b1;
                uint8_t b2;
                uint8_t b3;
                uint8_t b4;
        } byte;
        struct {
                uint16_t w1;
                uint16_t w2;
        } word;
} __attribute__((packed));
typedef union mem_ptr mem_ptr_t;

void *kzalloc(mm_superblock_t *sb, size_t memb, size_t count);
void  kzfree(mm_superblock_t *sb, void *ptr);
void  peek(size_t addr, int count);

#endif
