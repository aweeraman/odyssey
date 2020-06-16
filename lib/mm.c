/**
 * SPDX-FileCopyrightText: 2020 Anuradha Weeraman <anuradha@weeraman.com>
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include <lib/mm.h>
#include <lib/stdio.h>

#ifdef CONFIG_MM_FF
#include <mm/ff.h>
#endif

void *kzalloc(mm_superblock_t *sb, size_t memb, size_t count)
{
	return get_available_frame(sb, memb * count);
}

void kzfree(mm_superblock_t *sb, void *ptr)
{
	free_frame(sb, ptr);
}

void peek(size_t addr, int count)
{
	mem_ptr_t *p = (mem_ptr_t *) addr;

	// ARM supports both little-endian and big-endian with the former
	// being the norm
	for (int i = 0; i < count; i++) {
		printk("Peek addr=%x, %x    %x    %x    %x    %x    "
				 "%x    %x    %x        %c%c %c%c\n", p,
			p->nibble.n8, p->nibble.n7, p->nibble.n6, p->nibble.n5,
			p->nibble.n4, p->nibble.n3, p->nibble.n2, p->nibble.n1,
			PRINTABLE_CHAR(p->byte.b4), PRINTABLE_CHAR(p->byte.b3),
			PRINTABLE_CHAR(p->byte.b2), PRINTABLE_CHAR(p->byte.b1));
		printk("Peek addr=%x, %b%b%b%b %b%b%b%b %b%b%b%b %b%b%b%b "
				      "%b%b%b%b %b%b%b%b %b%b%b%b %b%b%b%b\n", p,
			p->bit.b31, p->bit.b30, p->bit.b29, p->bit.b28,
			p->bit.b27, p->bit.b26, p->bit.b25, p->bit.b24,
			p->bit.b23, p->bit.b22, p->bit.b21, p->bit.b20,
			p->bit.b19, p->bit.b18, p->bit.b17, p->bit.b16,
			p->bit.b15, p->bit.b14, p->bit.b13, p->bit.b12,
			p->bit.b11, p->bit.b10, p->bit.b9,  p->bit.b8,
			p->bit.b7,  p->bit.b6,  p->bit.b5,  p->bit.b4,
			p->bit.b3,  p->bit.b2,  p->bit.b1,  p->bit.b0);
		p += 1;
	}
}
