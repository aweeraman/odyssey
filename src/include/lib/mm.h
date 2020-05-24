/**
 * SPDX-FileCopyrightText: 2020 Anuradha Weeraman <anuradha@weeraman.com>
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef MM_H
#define MM_h

#include <stddef.h>
#include <mm/ff.h>

void *kalloc(mm_superblock_t *sb, size_t memb, size_t count);
void  kfree(mm_superblock_t *sb, void *ptr);

#endif
