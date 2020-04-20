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

#include "libk.h"

void panic(char *str) {
  printf("kernel panic: %s\n", str);
  while(1) {}
}

void printf(const char *fmt, ...) {
  int         i;
  size_t      ui;
  char       *str;
  char        print_buf[128];
  const char *p;

  va_list arg;
  va_start(arg, fmt);

  for(p = fmt; *p != '\0'; p++) {
    if(*p != '%') {
      printc(*p);
      continue;
    }

    switch(*++p) {
      case 'c':
        i = va_arg(arg, int);
        printc(i);
        break;

      case 's':
        str = va_arg(arg, char*);
        prints(str);
        break;

      case 'b':
        ui = va_arg(arg, size_t);
        prints(itoa((size_t) ui, print_buf, 2));
        break;

      case 'o':
        ui = va_arg(arg, size_t);
        prints(itoa((size_t) ui, print_buf, 8));
        break;

      case 'i':
      case 'd':
        i = va_arg(arg, int);
        if (i < 0) {
          i *= -1;
          printc('-');
        }
        prints(itoa((int) i, print_buf, 10));
        break;

      case 'u':
        ui = va_arg(arg, size_t);
        prints(itoa((size_t) ui, print_buf, 10));
        break;

      case 'x':
        ui = va_arg(arg, size_t);
        prints(itoa((size_t) ui, print_buf, 16));
        break;

      case '%':
        prints("%");
        break;
    }
  }

  va_end(arg);
}

size_t strnlen(const char *str, size_t maxlen) {
  size_t sz = 0;
  if (str == NULL) return 0;
  while (str[sz] != '\0' && sz++ < maxlen-1);
  return sz;
}

char* itoa(size_t value, char* result, int base) {
  // check that the base if valid
  if (base < 2 || base > 36) { *result = '\0'; return result; }

  char* ptr = result, *ptr1 = result, tmp_char;
  size_t tmp_value;

  do {
    tmp_value = value;
    value /= base;
    *ptr++ = "zyxwvutsrqponmlkjihgfedcba9876543210123456789abcdefghijklmnopqrstuvwxyz" [35 + (tmp_value - value * base)];
  } while ( value );

  *ptr-- = '\0';
  while(ptr1 < ptr) {
    tmp_char = *ptr;
    *ptr--= *ptr1;
    *ptr1++ = tmp_char;
  }
  return result;
}
