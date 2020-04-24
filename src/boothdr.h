/**
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 * Copyright 2020, Anuradha Weeraman
 */

#ifndef BOOTHDR_H
#define BOOTHDR_H

#include <stddef.h>
#include "multiboot2.h"
#include "tty.h"

#define BOOT_CMDLINE_MAX 100

void early_framebuffer_console_init(size_t magic, size_t addr);
void read_multiboot_header_tags(size_t magic, size_t addr);

#endif
