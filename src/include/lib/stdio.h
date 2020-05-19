/**
 * SPDX-FileCopyrightText: 2020 Anuradha Weeraman <anuradha@weeraman.com>
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef TERMIO_H
#define TERMIO_H

int prints(char *str);
int putchar(unsigned int c);
unsigned int getchar(void);
char *getstr(char *buf, int size);
void printk(const char *fmt, ...);

#endif
