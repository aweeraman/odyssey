/**
 * SPDX-FileCopyrightText: 2020 Anuradha Weeraman <anuradha@weeraman.com>
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef IO_H
#define IO_H

#include <stdint.h>

void     outb     (uint16_t port, uint8_t val);
void     outw     (uint16_t port, uint16_t val);
uint8_t  inb      (uint16_t port);
uint16_t inw      (uint16_t port);
void     io_wait  ();

#endif
