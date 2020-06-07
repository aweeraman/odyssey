/**
 * SPDX-FileCopyrightText: 2020 Anuradha Weeraman <anuradha@weeraman.com>
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef TERMIO_H
#define TERMIO_H

#include <stdint.h>

int     prints  (char *str);
uint8_t putchar (uint8_t c);
uint8_t getchar (void);
char   *getstr  (char *buf, int size);
void    printk  (const char *fmt, ...);

#endif
