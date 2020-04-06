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

#include "types.h"
#include "tty.h"
#include "serial.h"

cell *matrix = (cell *) 0xb8000;

static size_t cur_x = 0;
static size_t cur_y = 0;

void scroll() {
  for (size_t i=0; i<(TERMINAL_ROWS-1); i++) {
    for (size_t j=0; j<TERMINAL_COLS; j++) {
      matrix[(i*TERMINAL_COLS) + j].ch    = matrix[(i+1)*TERMINAL_COLS + j].ch;
      matrix[(i*TERMINAL_COLS) + j].clr   = matrix[(i+1)*TERMINAL_COLS + j].clr;
      matrix[(i+1)*TERMINAL_COLS + j].ch  = 0;
      matrix[(i+1)*TERMINAL_COLS + j].clr = 0;
    }
  }
}

void print_char(__uint8_t ch) {
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
    write_serial('\r');
    write_serial('\n');
    return;
  }
  matrix[(cur_x*TERMINAL_COLS) + cur_y++] = (cell) {
    .ch = ch,
    .clr = CLR_WHITE
  };
  write_serial(ch);
}

void clear(void) {
  for (size_t i=0; i<(TERMINAL_ROWS*TERMINAL_COLS); i++) {
    matrix[i].ch = 0;
    matrix[i].clr = 0;
  }
}

void print(char *str) {
  for (size_t i=0; str[i] != '\0'; i++) {
    print_char(str[i]);
  }
}

void println(char *str) {
  print(str);
  print("\n");
}
