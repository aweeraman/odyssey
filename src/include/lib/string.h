/**
 * SPDX-FileCopyrightText: 2020 Anuradha Weeraman <anuradha@weeraman.com>
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef STRING_H
#define STRING_H

#include <stdarg.h>
#include <stdint.h>
#include <stddef.h>

#define STRINGIFY2(str) #str
#define STRINGIFY(str) STRINGIFY2(str)

void *memcpy(void *dest, const void *src, size_t n);
void *memset(void *s, int c, size_t n);
char *strncpy(char *dest, const char *src, size_t n);
int strncmp(const char *s1, const char *s2, size_t n);
size_t strnlen(const char *s, size_t maxlen);
char *itoa(size_t value, char *result, int base);

#endif
