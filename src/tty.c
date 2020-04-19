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

#include <stdarg.h>
#include "tty.h"
#include "io.h"
#include "string.h"

#ifdef CONFIG_SERIAL
#include "serial.h"
#endif

static uint32_t cur_x = 0;
static uint32_t cur_y = 0;

static cell *matrix = (cell *) VGA_IO_ADDR;

void scroll() {
  for (uint32_t i=0; i<(TERMINAL_ROWS-1); i++) {
    for (uint32_t j=0; j<TERMINAL_COLS; j++) {
      matrix[(i*TERMINAL_COLS) + j].ch    = matrix[(i+1)*TERMINAL_COLS + j].ch;
      matrix[(i*TERMINAL_COLS) + j].clr   = matrix[(i+1)*TERMINAL_COLS + j].clr;
      matrix[(i+1)*TERMINAL_COLS + j].ch  = 0;
      matrix[(i+1)*TERMINAL_COLS + j].clr = 0;
    }
  }
}

void printc(uint8_t ch) {
  if (cur_y >= TERMINAL_COLS) {
    cur_x++;
    cur_y = 0;
  }
  if (cur_x >= TERMINAL_ROWS) {
    scroll();
    cur_x = TERMINAL_ROWS-1;
    cur_y = 0;
  }
  if (ch == '\n' || ch == '\r') {
    cur_x++;
    cur_y = 0;
    if (cur_x >= TERMINAL_ROWS) {
      scroll();
      cur_x = TERMINAL_ROWS-1;
    }
#ifdef CONFIG_SERIAL
    write_serial('\r');
    write_serial('\n');
#endif
    matrix[(cur_x*TERMINAL_COLS) + cur_y] = (cell) {
      .ch = 0,
      .clr = CLR_LIGHT_GREEN
    };
    update_cursor(cur_x, TERMINAL_COLS, cur_y);
    return;
  }
  matrix[(cur_x*TERMINAL_COLS) + cur_y++] = (cell) {
    .ch = ch,
    .clr = CLR_LIGHT_GREEN
  };
  matrix[(cur_x*TERMINAL_COLS) + cur_y] = (cell) {
    .ch = 0,
    .clr = CLR_LIGHT_GREEN
  };
  update_cursor(cur_x, TERMINAL_COLS, cur_y);
#ifdef CONFIG_SERIAL
  write_serial(ch);
#endif
}

void clear(void) {
  for (uint32_t i=0; i<(TERMINAL_ROWS*TERMINAL_COLS); i++) {
    matrix[i].ch = 0;
    matrix[i].clr = 0;
  }
}

void prints(char *str) {
  for (uint32_t i=0; str[i] != '\0'; i++) {
    printc(str[i]);
  }
}

void init_console() {
  clear();
  enable_cursor(1, 15);
}

void printk(const char *fmt, ...) {
  int32_t i;
  uint32_t l;
  char *str;
  char print_buf[128];
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

      case 'd':
        i = va_arg(arg, int32_t);
        if (i < 0) {
          i *= -1;
          printc('-');
        }
        prints(uitoa((int32_t) i, print_buf, 10));
        break;

      case 's':
        str = va_arg(arg, char*);
        prints(str);
        break;

      case 'u':
        l = va_arg(arg, uint32_t);
        prints(uitoa((uint32_t) l, print_buf, 16));
        break;

      case '%':
        prints("%");
        break;
    }
  }

  va_end(arg);
}
