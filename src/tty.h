/**
 * SPDX-FileCopyrightText: 2020 Anuradha Weeraman <anuradha@weeraman.com>
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef TTY_H
#define TTY_H

#include <stdint.h>
#include <stddef.h>
#include <stdarg.h>
#include <serial.h>
#include <x86/io.h>
#include <kernel.h>
#include <boot/multiboot2.h>
#include <libk.h>

#define CLR_BLACK         0
#define CLR_BLUE          1
#define CLR_GREEN         2
#define CLR_CYAN          3
#define CLR_RED           4
#define CLR_MAGENTA       5
#define CLR_BROWN         6
#define CLR_LIGHT_GREY    7
#define CLR_DARK_GREY     8
#define CLR_LIGHT_BLUE    9
#define CLR_LIGHT_GREEN   10
#define CLR_LIGHT_CYAN    11
#define CLR_LIGHT_RED     12
#define CLR_LIGHT_MAGENTA 13
#define CLR_LIGHT_BROWN   14
#define CLR_WHITE         15

#define RGB_BLACK         0
#define RGB_WHITE         0xffffffff
#define RGB_CYAN          0x0000ffff
#define RGB_FG            RGB_CYAN

#define UNICODE_CURSOR    0x2588

#define FB_RGB (framebuffer->common.framebuffer_type \
		== MULTIBOOT_FRAMEBUFFER_TYPE_RGB)
#define FB_EGA (framebuffer->common.framebuffer_type \
		== MULTIBOOT_FRAMEBUFFER_TYPE_EGA_TEXT)

typedef struct {
        uint8_t ch;
        uint8_t clr;
} __attribute__((packed)) cell;

void     init_console();
void     enable_cursor(uint8_t cursor_start, uint8_t cursor_end);
void     disable_cursor();
void     blink_cursor();
void     update_cursor(uint8_t x, uint8_t width, uint8_t y);
uint16_t get_cursor_position(void);
void     printc(uint8_t ch);
void     backspace();
void     clear_screen(void);

#endif
