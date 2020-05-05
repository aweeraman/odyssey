/**
 * SPDX-FileCopyrightText: 2020 Anuradha Weeraman <anuradha@weeraman.com>
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef LIBK_H
#define LIBK_H

#include <stdarg.h>
#include <stdint.h>
#include <stddef.h>
#include "tty.h"

void*  memcpy(void *dest, const void *src, size_t n);
void*  memset(void *s, int c, size_t n);
char*  strncpy(char *dest, const char *src, size_t n);
int    strncmp(const char *s1, const char *s2, size_t n);
int    prints(char *str);
int    putchar(int c);
char*  itoa(size_t value, char *result, int base);
size_t strnlen(const char *s, size_t maxlen);
void   printf(const char *fmt, ...);
void   panic(char *str);

#endif
