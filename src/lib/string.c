/**
 * SPDX-FileCopyrightText: 2020 Anuradha Weeraman <anuradha@weeraman.com>
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 * C++ version 0.4 char* style "itoa", Copyright Lukás Chmela
 * Released under GPLv3. http://www.strudel.org.uk/itoa/
 */

#include <lib/string.h>

void* memcpy(void * restrict d, const void * restrict s, size_t n)
{
	char *dest = d;
	const char *src = s;

	for (size_t i = 0; i < n; i++) {
		dest[i] = src[i];
	}

	return dest;
}

void* memset(void *s, int c, size_t n)
{
	unsigned char *ch = s;
	for (size_t i = 0; i < n; i++)
		ch[i] = (char) c;
	return s;
}

size_t strnlen(const char *str, size_t maxlen)
{
	size_t sz = 0;
	if (str == NULL)
		return 0;
	while (str[sz] != '\0' && sz++ < maxlen-1);
	return sz;
}

char* strncpy(char * restrict dest, const char * restrict src, size_t n)
{
	size_t i = 0;

	for (; src[i] != '\0' && i < n; i++)
		dest[i] = src[i];

	for (; i < n; i++)
		dest[i] = '\0';

	return dest;
}

int strncmp(const char *s1, const char *s2, size_t n)
{
	for (size_t i = 0; i < n; i++) {
		if (s1[i] == '\0' && s2[i] == '\0') {
			return 0;
		} else {
			if (s1[i] != s2[i]) {
				if (s1[i] < s2[i])
					return -1;
				else
					return 1;
			}
		}
	}

	return 0;
}

char* itoa(size_t value, char* result, int base)
{
	// check that the base if valid
	if (base < 2 || base > 36) {
		*result = '\0';
		return result;
	}

	char* ptr = result, *ptr1 = result, tmp_char;
	size_t tmp_value;

	do {
		tmp_value = value;
		value /= base;
		*ptr++ = "zyxwvutsrqponmlkjihgfedcba9876543210123456789"
			"abcdefghijklmnopqrstuvwxyz" [35 + (tmp_value - value * base)];
	} while ( value );

	*ptr-- = '\0';
	while(ptr1 < ptr) {
		tmp_char = *ptr;
		*ptr--= *ptr1;
		*ptr1++ = tmp_char;
	}
	return result;
}
