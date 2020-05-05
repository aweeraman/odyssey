/**
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 * Copyright 2020, Anuradha Weeraman
 */

#ifndef IO_H
#define IO_H

#include <stdint.h>

#define VGA_IDX_PORT  0x3D4
#define VGA_DATA_PORT (VGA_IDX_PORT+1)

void    outb(uint16_t port, uint8_t val);
uint8_t inb(uint16_t port);
void    io_wait();

#endif
