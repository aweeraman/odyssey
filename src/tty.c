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
#include <stddef.h>
#include "tty.h"
#include "io.h"
#include "kernel.h"
#include "multiboot2.h"

#ifdef CONFIG_SERIAL
#include "serial.h"
#endif

static size_t cur_x = 0;
static size_t cur_y = 0;

static cell *matrix = NULL;
struct multiboot_tag_framebuffer_common *framebuffer;

static void scroll() {
  for (size_t i=0; i<(TERMINAL_ROWS-1); i++) {
    for (size_t j=0; j<TERMINAL_COLS; j++) {
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

void clear_screen(void) {
  for (size_t i = 0; i < (TERMINAL_ROWS * TERMINAL_COLS); i++) {
    matrix[i].ch = 0;
    matrix[i].clr = 0;
  }
}

void init_console() {
  if (framebuffer->framebuffer_type == MULTIBOOT_FRAMEBUFFER_TYPE_RGB) {
    unsigned* fb = (unsigned*) framebuffer->framebuffer_addr;
    for (size_t i = 0; i < (framebuffer->framebuffer_width *
			    framebuffer->framebuffer_height); i++) {
        fb[i] = 0x0000ff00;
    }
    printf("Initialized RGB video at 0x%x\n", framebuffer->framebuffer_addr);
  } else if (framebuffer->framebuffer_type == MULTIBOOT_FRAMEBUFFER_TYPE_EGA_TEXT) {
    matrix = (cell *) framebuffer->framebuffer_addr;
    clear_screen();
    enable_cursor(1, 15);
    printf("Initialized EGA video at 0x%x\n", framebuffer->framebuffer_addr);
  } else {
    printf("Video mode %d not supported\n", framebuffer->framebuffer_type);
  }
}
