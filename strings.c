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
 *
 * C++ version 0.4 char* style "itoa", Copyright Lukás Chmela
 * Released under GPLv3.
 * http://www.strudel.org.uk/itoa/
 */

#include "strings.h"
#include "print.h"

size_t strlen(char *str) {
  size_t sz = 0;
  while (str[sz++] != '\0');
  return sz;
}

char* itoa(int value, char* result, int base) {
  // check that the base if valid
  if (base < 2 || base > 36) { *result = '\0'; return result; }

  char* ptr = result, *ptr1 = result, tmp_char;
  int tmp_value;

  do {
    tmp_value = value;
    value /= base;
    *ptr++ = "zyxwvutsrqponmlkjihgfedcba9876543210123456789abcdefghijklmnopqrstuvwxyz" [35 + (tmp_value - value * base)];
  } while ( value );

  // Apply negative sign
  if (tmp_value < 0) *ptr++ = '-';
  *ptr-- = '\0';
  while(ptr1 < ptr) {
    tmp_char = *ptr;
    *ptr--= *ptr1;
    *ptr1++ = tmp_char;
  }
  return result;
}

void printf(const char *fmt, ...) {
  const char *p;
  char print_buf[128];
  char **arg = (char **) &fmt;

  for(p = fmt; *p != '\0'; p++) {
    if(*p != '%') {
      print_char(*p);
      continue;
    }

    switch(*++p) {
      case 'c':
        print_char((size_t) *(++arg));
        break;

      case 'd':
        print(itoa((size_t) *(++arg), print_buf, 10));
        break;

      case 's':
        print((char *) *(++arg));
        break;

      case 'x':
        print(itoa((size_t) *(++arg), print_buf, 16));
        break;

      case '%':
        print("%");
        break;
    }
  }
}