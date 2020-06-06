/**
 * SPDX-FileCopyrightText: 2020 Anuradha Weeraman <anuradha@weeraman.com>
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include <sys/tty.h>
#include <sys/timer.h>
#include <lib/stdio.h>
#include <x86/boot/multiboot2.h>
#include <x86/io.h>
#include <stddef.h>

#ifdef CONFIG_SERIAL
#include <sys/serial.h>
#endif

#ifdef CONFIG_FRAMEBUFFER_RGB
#define SSFN_NOIMPLEMENTATION
#define SSFN_CONSOLEBITMAP_TRUECOLOR
#include <sys/ssfn.h>
#define FONT_HEIGHT 16
#define FONT_WIDTH  8
#endif

#define VGA_IDX_PORT  0x3D4
#define VGA_DATA_PORT (VGA_IDX_PORT+1)

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

#define CLR_FG            ((CLR_BLUE << 4) | (CLR_LIGHT_GREY & 0x0f))
#define RGB_BG            0x000a3b7a
#define RGB_FG            0x00dceafc

#define UNICODE_CURSOR    0x2588

#define IS_RGB (framebuffer.type == MULTIBOOT_FRAMEBUFFER_TYPE_RGB)
#define IS_EGA (framebuffer.type == MULTIBOOT_FRAMEBUFFER_TYPE_EGA_TEXT)

typedef struct {
        uint8_t ch;
        uint8_t clr;
} __attribute__((packed)) cell;

extern char _binary_sys_f_sfn_start;

static uint32_t counter = 0;

static cell  *matrix = NULL;
static size_t rows  = 0;
static size_t cols  = 0;
static size_t cur_x = 0;
static size_t cur_y = 0;

static framebuffer_t framebuffer;

void enable_cursor(uint8_t cursor_start, uint8_t cursor_end)
{
        if (IS_EGA) {
                outb(VGA_IDX_PORT, 0x0A);
                outb(VGA_DATA_PORT, (inb(VGA_DATA_PORT) & 0xC0) | cursor_start);

                outb(VGA_IDX_PORT, 0x0B);
                outb(VGA_DATA_PORT, (inb(VGA_DATA_PORT) & 0xE0) | cursor_end);
        }
}

void disable_cursor()
{
        if (IS_EGA) {
                outb(VGA_IDX_PORT, 0x0A);
                outb(VGA_DATA_PORT, 0x20);
        }
}

static void scroll()
{
        if (IS_RGB) {
#ifdef CONFIG_FRAMEBUFFER_RGB
                for (size_t i = 0; i < framebuffer.height-16; i++) {
                        for (size_t j = 0; j < framebuffer.width; j++) {
                                framebuffer.addr[(i*framebuffer.width) + j] =
                                        framebuffer.addr[((i+16) * framebuffer.width) + j];
                        }
                }
                for (size_t i = framebuffer.height-16; i < framebuffer.height; i++) {
                        for (size_t j = 0; j < framebuffer.width; j++) {
                                draw_pixel(i, j, RGB_BG);
                        }
                }
#endif
        } else if (IS_EGA) {
                for (size_t i = 0; i < (rows - 1); i++) {
                        for (size_t j = 0; j < cols; j++) {
                                matrix[(i*cols) + j].ch    = matrix[(i+1)*cols + j].ch;
                                matrix[(i*cols) + j].clr   = matrix[(i+1)*cols + j].clr;
                                matrix[(i+1)*cols + j].ch  = 0;
                                matrix[(i+1)*cols + j].clr = CLR_FG;
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

        if (IS_RGB) {
#ifdef CONFIG_FRAMEBUFFER_RGB
                ssfn_y  = x * FONT_HEIGHT;
                ssfn_x  = y * FONT_WIDTH;
                if (counter % 2 == 0)
                        ssfn_fg = RGB_FG;
                else
                        ssfn_fg = RGB_BG;
                ssfn_putc(UNICODE_CURSOR);
#endif
        } else if (IS_EGA) {
                outb(VGA_IDX_PORT, 0x0F);
                outb(VGA_DATA_PORT, (uint8_t) (pos & 0xFF));
                outb(VGA_IDX_PORT, 0x0E);
                outb(VGA_DATA_PORT, (uint8_t) ((pos >> 8) & 0xFF));
        }
}

uint16_t get_cursor_position(void)
{
        uint16_t pos = 0;

        if (IS_EGA) {
                outb(VGA_IDX_PORT, 0x0F);
                pos |= inb(VGA_DATA_PORT);
                outb(VGA_IDX_PORT, 0x0E);
                pos |= ((uint16_t) inb(VGA_DATA_PORT)) << 8;
        }

        return pos;
}

static void write_character(uint8_t c)
{

#ifdef CONFIG_FRAMEBUFFER_RGB
        if (IS_RGB) {
                ssfn_y = cur_x * FONT_HEIGHT;
                ssfn_x = cur_y * FONT_WIDTH;
                ssfn_fg = RGB_BG;
                ssfn_putc(UNICODE_CURSOR);

                ssfn_y = cur_x * FONT_HEIGHT;
                ssfn_x = cur_y * FONT_WIDTH;
                ssfn_fg = RGB_FG;
                ssfn_putc(c);
        }
#else
        if (IS_EGA) {
                matrix[(cur_x*cols) + cur_y] = (cell) {
                        .ch = c,
                        .clr = CLR_FG
                };
                matrix[(cur_x*cols) + cur_y+1] = (cell) {
                        .ch = 0,
                        .clr = CLR_FG
                };
                update_cursor(cur_x, cols, cur_y+1);
        }
#endif

#ifdef CONFIG_SERIAL
        write_serial(c);
#endif
}

static void write_newline()
{
#ifdef CONFIG_FRAMEBUFFER_RGB
        if (IS_RGB) {
                ssfn_y = cur_x * FONT_HEIGHT;
                ssfn_x = cur_y * FONT_WIDTH;
                ssfn_fg = RGB_BG;
                ssfn_putc(UNICODE_CURSOR);
        }
#else
        if (IS_EGA) {
                matrix[(cur_x*cols) + cur_y] = (cell) {
                        .ch = 0,
                        .clr = CLR_FG
                };
                update_cursor(cur_x, cols, cur_y);
        }
#endif

#ifdef CONFIG_SERIAL
        write_serial('\r');
        write_serial('\n');
#endif

}

void printc(uint8_t ch)
{
        if (cur_y >= cols) {
                cur_x++;
                cur_y = 0;
        }

        if (cur_x >= rows) {
                scroll();
                cur_x = rows - 1;
                cur_y = 0;
        }

        if (ch == '\b') {
                backspace();
                return;
        }

        if (ch == '\n' || ch == '\r') {
                write_newline();
                cur_x++;
                cur_y = 0;
                if (cur_x >= rows) {
                        scroll();
                        cur_x = rows - 1;
                }
                return;
        }

        write_character(ch);
        cur_y++;

        if (cur_y >= cols) {
                cur_x++;
                cur_y = 0;
        }

        if (cur_x >= rows) {
                scroll();
                cur_x = rows - 1;
                cur_y = 0;
        }
}

#ifdef CONFIG_FRAMEBUFFER_RGB
void draw_pixel(int x, int y, uint32_t color)
{
        framebuffer.addr[x * framebuffer.width + y] = color;
}
#endif

void backspace()
{
        if (IS_RGB) {
#ifdef CONFIG_FRAMEBUFFER_RGB
                if (cur_y > 0) {
                        ssfn_y = cur_x * FONT_HEIGHT;
                        ssfn_x = cur_y * FONT_WIDTH;
                        ssfn_fg = RGB_BG;
                        ssfn_putc(UNICODE_CURSOR);

                        cur_y--;

                        ssfn_y = cur_x * FONT_HEIGHT;
                        ssfn_x = cur_y * FONT_WIDTH;
                        ssfn_fg = RGB_FG;
                        ssfn_putc(UNICODE_CURSOR);
                }
#endif
        } else if (IS_EGA) {
                if (cur_y > 0) {
                        cur_y--;
                        int pos = (cur_x * cols) + cur_y;
                        matrix[pos].ch = 0;
                        matrix[pos].clr = CLR_FG;
                }
        }
        update_cursor(cur_x, cols, cur_y);
#ifdef CONFIG_SERIAL
        write_serial('\b');
#endif
}

void clear_screen(void)
{
        if (IS_RGB) {
#ifdef CONFIG_FRAMEBUFFER_RGB
                for (size_t x = 0; x < framebuffer.height; x++) {
                        for (size_t y = 0; y < framebuffer.width; y++) {
                                draw_pixel(x, y, RGB_BG);
                        }
                }
                cur_x = 0;
                cur_y = 0;
#endif
        } else if (IS_EGA) {
                for (size_t i = 0; i < (rows * cols); i++) {
                        matrix[i].ch = 0;
                        matrix[i].clr = CLR_FG;
                }
                cur_x = 0;
                cur_y = 0;
        }
}

void init_console(framebuffer_t fb_init)
{

        framebuffer = fb_init;

        if (IS_RGB) {
#ifdef CONFIG_FRAMEBUFFER_RGB
                rows = framebuffer.height / 16;
                cols = framebuffer.width  / 8;

                clear_screen();

                //Setup scalable font library
                ssfn_font = (ssfn_font_t *) &_binary_sys_f_sfn_start;
                ssfn_dst_ptr = (uint8_t *) (size_t) framebuffer.addr;
                ssfn_dst_pitch = framebuffer.pitch;
                ssfn_fg = 0xFFFFFFFF;
                ssfn_x = 0;
                ssfn_y = 0;

                printk("Initialized RGB framebuffer at 0x%x\n",
                        framebuffer.addr);

                printk("Video addr=0x%X pitch=%d width=%d height=%d bpp=%d type=%d\n",
                        framebuffer.addr, framebuffer.pitch, framebuffer.width,
                        framebuffer.height, framebuffer.bpp, framebuffer.type);
#endif
        } else if (IS_EGA) {
                matrix = (cell *) (size_t) framebuffer.addr;
                rows = framebuffer.height;
                cols = framebuffer.width;
                clear_screen();
                enable_cursor(1, 15);

                printk("Initialized EGA framebuffer at 0x%X width=%d height=%d\n",
                        framebuffer.addr, framebuffer.width, framebuffer.height);
        } else {
                printk("Framebuffer type %d not supported\n", framebuffer.type);
        }
}
