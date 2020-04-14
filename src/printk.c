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

#include "printk.h"

// Reference: https://www.eskimo.com/~scs/cclass/int/sx11b.html
void printk(const char *fmt, ...) {
  const char *p;
  char print_buf[128];
  char **arg = (char **) &fmt;

  for(p = fmt; *p != '\0'; p++) {
    if(*p != '%') {
      printc(*p);
      continue;
    }

    switch(*++p) {
      case 'c':
        printc((uint32_t) *(++arg));
        break;

      case 'd':
        // TODO: replace with a signed implementation
        prints(uitoa((uint32_t) *(++arg), print_buf, 10));
        break;

      case 's':
        prints((char *) *(++arg));
        break;

      case 'u':
        prints(uitoa((uint32_t) *(++arg), print_buf, 16));
        break;

      case '%':
        prints("%");
        break;
    }
  }
}
