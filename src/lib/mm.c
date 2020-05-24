/**
 * SPDX-FileCopyrightText: 2020 Anuradha Weeraman <anuradha@weeraman.com>
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include <lib/mm.h>
#include <lib/stdio.h>

#ifdef CONFIG_MM_FF
#include <mm/ff.h>
#endif

void *kalloc(mm_superblock_t *sb, size_t memb, size_t count)
{
        return get_available_frame(sb, memb * count);
}

void kfree(mm_superblock_t *sb, void *ptr)
{
        free_frame(sb, ptr);
}

void peek(size_t addr, int count)
{
        mem_ptr_t *p = (mem_ptr_t *) addr;

#ifdef ARCH_X86
        for (int i = 0; i < count; i++) {
                printk("Peek addr=%x, %x%x%x%x %x%x%x%x %c%c %c%c\n", p,
                        p->nibble.n4, p->nibble.n3, p->nibble.n2, p->nibble.n1,
                        p->nibble.n8, p->nibble.n7, p->nibble.n6, p->nibble.n5,
                        PRINTABLE_CHAR(p->byte.b2), PRINTABLE_CHAR(p->byte.b1),
                        PRINTABLE_CHAR(p->byte.b4), PRINTABLE_CHAR(p->byte.b3));
                p += 1;
        }
#endif
}
