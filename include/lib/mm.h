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
		uint8_t b0 : 1;
		uint8_t b1 : 1;
		uint8_t b2 : 1;
		uint8_t b3 : 1;
		uint8_t b4 : 1;
		uint8_t b5 : 1;
		uint8_t b6 : 1;
		uint8_t b7 : 1;
		uint8_t b8 : 1;
		uint8_t b9 : 1;
		uint8_t b10: 1;
		uint8_t b11: 1;
		uint8_t b12: 1;
		uint8_t b13: 1;
		uint8_t b14: 1;
		uint8_t b15: 1;
		uint8_t b16: 1;
		uint8_t b17: 1;
		uint8_t b18: 1;
		uint8_t b19: 1;
		uint8_t b20: 1;
		uint8_t b21: 1;
		uint8_t b22: 1;
		uint8_t b23: 1;
		uint8_t b24: 1;
		uint8_t b25: 1;
		uint8_t b26: 1;
		uint8_t b27: 1;
		uint8_t b28: 1;
		uint8_t b29: 1;
		uint8_t b30: 1;
		uint8_t b31: 1;
	} bit;
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
void kzfree(mm_superblock_t *sb, void *ptr);
void peek(size_t addr, int count);

#endif
