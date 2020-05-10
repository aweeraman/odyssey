/**
 * SPDX-FileCopyrightText: 2020 Anuradha Weeraman <anuradha@weeraman.com>
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include <sys/tty.h>
#include <sys/timer.h>
#include <lib/termio.h>

#ifdef CONFIG_SERIAL
#include <sys/serial.h>
#endif

#ifdef CONFIG_FRAMEBUFFER_RGB
#define SSFN_NOIMPLEMENTATION
#define SSFN_CONSOLEBITMAP_TRUECOLOR
#include <sys/ssfn.h>

extern char _binary_sys_f_sfn_start;

static size_t *fb = NULL;
static size_t fb_height;
static size_t fb_width;
#endif

static uint32_t counter = 0;

static cell *matrix = NULL;
static size_t rows  = 0;
static size_t cols  = 0;
static size_t cur_x = 0;
static size_t cur_y = 0;

struct multiboot_tag_framebuffer *framebuffer;

void enable_cursor(uint8_t cursor_start, uint8_t cursor_end)
{
        if (FB_EGA) {
                outb(VGA_IDX_PORT, 0x0A);
                outb(VGA_DATA_PORT, (inb(VGA_DATA_PORT) & 0xC0) | cursor_start);

                outb(VGA_IDX_PORT, 0x0B);
                outb(VGA_DATA_PORT, (inb(VGA_DATA_PORT) & 0xE0) | cursor_end);
        }
}

void disable_cursor()
{
        if (FB_EGA) {
                outb(VGA_IDX_PORT, 0x0A);
                outb(VGA_DATA_PORT, 0x20);
        }
}

static void scroll()
{
        if (FB_RGB) {
#ifdef CONFIG_FRAMEBUFFER_RGB
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
#endif
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

void blink_cursor()
{
        update_cursor(cur_x, cols, cur_y);
}

void update_cursor(uint8_t x, uint8_t width, uint8_t y)
{
        uint16_t pos = (x * width) + y;
        counter++;

        if (FB_RGB) {
#ifdef CONFIG_FRAMEBUFFER_RGB
                ssfn_y  = x*16;
                ssfn_x  = y*8;
                if (counter % 2 == 0)
                        ssfn_fg = RGB_FG;
                else
                        ssfn_fg = RGB_BLACK;
                ssfn_putc(UNICODE_CURSOR);
#endif
        } else if (FB_EGA) {
                outb(VGA_IDX_PORT, 0x0F);
                outb(VGA_DATA_PORT, (uint8_t) (pos & 0xFF));
                outb(VGA_IDX_PORT, 0x0E);
                outb(VGA_DATA_PORT, (uint8_t) ((pos >> 8) & 0xFF));
        }
}

uint16_t get_cursor_position(void)
{
        uint16_t pos = 0;

        if (FB_EGA) {
                outb(VGA_IDX_PORT, 0x0F);
                pos |= inb(VGA_DATA_PORT);
                outb(VGA_IDX_PORT, 0x0E);
                pos |= ((uint16_t) inb(VGA_DATA_PORT)) << 8;
        }

        return pos;
}

void printc(uint8_t ch)
{
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
                if (FB_RGB) {
#ifdef CONFIG_FRAMEBUFFER_RGB
                        ssfn_y = cur_x*16;
                        ssfn_x = cur_y*8;
                        ssfn_fg = RGB_BLACK;
                        ssfn_putc(UNICODE_CURSOR);
#endif
                }
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
#ifdef CONFIG_FRAMEBUFFER_RGB
                ssfn_y = cur_x*16;
                ssfn_x = cur_y*8;
                ssfn_fg = RGB_BLACK;
                ssfn_putc(UNICODE_CURSOR);

                ssfn_y = cur_x*16;
                ssfn_x = cur_y*8;
                ssfn_fg = RGB_FG;
                ssfn_putc(ch);

                cur_y++;

                if (cur_y >= cols) {
                        cur_x++;
                        cur_y = 0;
                }
                if (cur_x >= rows) {
                        scroll();
                        cur_x = rows-1;
                        cur_y = 0;
                }
#endif
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

#ifdef CONFIG_FRAMEBUFFER_RGB
void draw_pixel(int x, int y, int color)
{
        fb[x * fb_width + y ] = color;
}
#endif

void backspace()
{
        if (FB_RGB) {
#ifdef CONFIG_FRAMEBUFFER_RGB
                if (cur_y > 0) {
                        ssfn_y = cur_x*16;
                        ssfn_x = cur_y*8;
                        ssfn_fg = RGB_BLACK;
                        ssfn_putc(UNICODE_CURSOR);

                        cur_y--;

                        ssfn_y = cur_x*16;
                        ssfn_x = cur_y*8;
                        ssfn_fg = RGB_WHITE;
                        ssfn_putc(UNICODE_CURSOR);
                }
#endif
        } else if (FB_EGA) {
                if (cur_y > 0) {
                        cur_y--;
                        int pos = (cur_x * cols) + cur_y;
                        matrix[pos].ch = 0;
                        matrix[pos].clr = CLR_LIGHT_GREEN;
                }
        }
        update_cursor(cur_x, cols, cur_y);
#ifdef CONFIG_SERIAL
        write_serial('\b');
#endif
}

void clear_screen(void)
{
        if (FB_RGB) {
#ifdef CONFIG_FRAMEBUFFER_RGB
                for (size_t x = 0; x < fb_height; x++) {
                        for (size_t y = 0; y < fb_width; y++) {
                                draw_pixel(x, y, 0x00000000);
                        }
                }
#endif
        } else if (FB_EGA) {
                for (size_t i = 0; i < (rows * cols); i++) {
                        matrix[i].ch = 0;
                        matrix[i].clr = 0;
                }
        }
}

void init_console()
{
        if (FB_RGB) {
#ifdef CONFIG_FRAMEBUFFER_RGB
        fb = (size_t *) (size_t) framebuffer->common.framebuffer_addr;

        fb_height = framebuffer->common.framebuffer_height;
        fb_width  = framebuffer->common.framebuffer_width;
        rows = fb_height / 16;
        cols = fb_width  / 8;

        clear_screen();

        //Setup scalable font library
        ssfn_font = (ssfn_font_t *) &_binary_sys_f_sfn_start;
        ssfn_dst_ptr = (uint8_t *) (size_t) framebuffer->common.framebuffer_addr;
        ssfn_dst_pitch = framebuffer->common.framebuffer_pitch;
        ssfn_fg = 0xFFFFFFFF;
        ssfn_x = 0;
        ssfn_y = 0;

        printf("Initialized RGB framebuffer at 0x%x\n",
                        framebuffer->common.framebuffer_addr);
#endif
        } else if (FB_EGA) {
                matrix = (cell *) (size_t) framebuffer->common.framebuffer_addr;
                rows = framebuffer->common.framebuffer_height;
                cols = framebuffer->common.framebuffer_width;
                clear_screen();
                enable_cursor(1, 15);

                printf("Initialized EGA framebuffer at 0x%x\n",
                                framebuffer->common.framebuffer_addr);
        } else {
                printf("Framebuffer type %d not supported\n",
                                framebuffer->common.framebuffer_type);
        }
}
