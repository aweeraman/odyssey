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

#include "tty.h"

#define SSFN_NOIMPLEMENTATION
#define SSFN_CONSOLEBITMAP_TRUECOLOR
#include "ssfn.h"

#ifdef CONFIG_SERIAL
#include "serial.h"
#endif

extern char _binary_unifont_sfn_start;

static cell *matrix = NULL;
static size_t rows  = 0;
static size_t cols  = 0;
static size_t cur_x = 0;
static size_t cur_y = 0;

static size_t *fb = NULL;
static size_t fb_height;
static size_t fb_width;
struct multiboot_tag_framebuffer *framebuffer;

static void scroll() {

  if (FB_RGB) {
    for (size_t i = 0; i < fb_height-16; i++) {
      for (size_t j = 0; j < fb_width; j++) {
	fb[(i*fb_width) + j] = fb[((i+16) * fb_width) + j];
      }
    }
    for (size_t i = fb_height-16; i < fb_height; i++) {
      for (size_t j = 0; j < fb_width; j++) {
	fb[(i*fb_width) + j] = 0;
      }
    }
  } else if (FB_EGA) {
    for (size_t i = 0; i < (rows - 1); i++) {
      for (size_t j = 0; j < cols; j++) {
        matrix[(i*cols) + j].ch    = matrix[(i+1)*cols + j].ch;
        matrix[(i*cols) + j].clr   = matrix[(i+1)*cols + j].clr;
        matrix[(i+1)*cols + j].ch  = 0;
        matrix[(i+1)*cols + j].clr = 0;
      }
    }
  }
}

void printc(uint8_t ch) {
  if (cur_y >= cols) {
    cur_x++;
    cur_y = 0;
  }
  if (cur_x >= rows) {
    scroll();
    cur_x = rows-1;
    cur_y = 0;
  }
  if (ch == '\n' || ch == '\r') {
    cur_x++;
    cur_y = 0;
    if (cur_x >= rows) {
      scroll();
      cur_x = rows-1;
    }
#ifdef CONFIG_SERIAL
    write_serial('\r');
    write_serial('\n');
#endif
    if (FB_EGA) {
      matrix[(cur_x*cols) + cur_y] = (cell) {
        .ch = 0,
        .clr = CLR_LIGHT_GREEN
      };
      update_cursor(cur_x, cols, cur_y);
    }
    return;
  }
  if (FB_RGB) {
    ssfn_y = cur_x*16;
    ssfn_x = cur_y*8;
    cur_y++;
    ssfn_putc(ch);
  } else if (FB_EGA) {
    matrix[(cur_x*cols) + cur_y++] = (cell) {
      .ch = ch,
      .clr = CLR_LIGHT_GREEN
    };
    matrix[(cur_x*cols) + cur_y] = (cell) {
      .ch = 0,
      .clr = CLR_LIGHT_GREEN
    };
  }
  update_cursor(cur_x, cols, cur_y);
#ifdef CONFIG_SERIAL
  write_serial(ch);
#endif
}

static void draw_pixel(int x, int y, int color) {
  fb[x * fb_width + y ] = color;
}

void clear_screen(void) {
  if (FB_RGB) {
    for (size_t x = 0; x < fb_height; x++) {
      for (size_t y = 0; y < fb_width; y++) {
        draw_pixel(x, y, 0x00000000);
      }
    }
  } else if (FB_EGA) {
    for (size_t i = 0; i < (rows * cols); i++) {
      matrix[i].ch = 0;
      matrix[i].clr = 0;
    }
  }
}

void init_console() {
  if (FB_RGB) {

    fb = (size_t *) framebuffer->common.framebuffer_addr;
    fb_height = framebuffer->common.framebuffer_height;
    fb_width  = framebuffer->common.framebuffer_width;
    rows = fb_height / 16;
    cols = fb_width  / 8;

    clear_screen();

    //Setup scalable font library
    ssfn_font = &_binary_unifont_sfn_start;
    ssfn_dst_ptr = framebuffer->common.framebuffer_addr;
    ssfn_dst_pitch = framebuffer->common.framebuffer_pitch;
    ssfn_fg = 0xFFFF;
    ssfn_x = 0;
    ssfn_y = 0;

    printf("Initialized RGB framebuffer at 0x%x\n", framebuffer->common.framebuffer_addr);

  } else if (FB_EGA) {

    matrix = (cell *) framebuffer->common.framebuffer_addr;
    rows = framebuffer->common.framebuffer_height;
    cols = framebuffer->common.framebuffer_width;
    clear_screen();
    enable_cursor(1, 15);

    printf("Initialized EGA framebuffer at 0x%x\n", framebuffer->common.framebuffer_addr);

  } else {
    printf("Framebuffer type %d not supported\n", framebuffer->common.framebuffer_type);
  }
}
