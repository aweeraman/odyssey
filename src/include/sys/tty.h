/**
 * SPDX-FileCopyrightText: 2020 Anuradha Weeraman <anuradha@weeraman.com>
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef TTY_H
#define TTY_H

#include <stdint.h>

typedef struct fb_info {
        uint8_t  type;
        uint8_t  bpp;
        uint64_t addr;
        uint32_t width;
        uint32_t height;
        uint32_t pitch;

} framebuffer_t;

void     init_console(framebuffer_t framebuffer);
void     enable_cursor(uint8_t cursor_start, uint8_t cursor_end);
void     disable_cursor();
void     blink_cursor();
void     update_cursor(uint8_t x, uint8_t width, uint8_t y);
uint16_t get_cursor_position(void);
void     printc(uint8_t ch);
void     draw_pixel(int x, int y, uint32_t color);
void     backspace();
void     clear_screen(void);

#endif
