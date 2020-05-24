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
