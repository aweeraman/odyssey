/**
 * SPDX-FileCopyrightText: 2020 Anuradha Weeraman <anuradha@weeraman.com>
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef BOOTHDR_H
#define BOOTHDR_H

#include <stddef.h>
#include <boot/multiboot2.h>
#include <tty.h>

#define BOOT_CMDLINE_MAX 100

void early_framebuffer_console_init(size_t magic, size_t addr);
void read_multiboot_header_tags(size_t magic, size_t addr);

#endif
