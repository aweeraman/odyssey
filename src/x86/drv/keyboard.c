/**
 * SPDX-FileCopyrightText: 2020 Anuradha Weeraman <anuradha@weeraman.com>
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 * KBDUS scancode table, Copyright Brandon F. (friesenb@gmail.com)
 */

#include <sys/keyboard.h>
#include <sys/tty.h>
#include <x86/io.h>
#include <lib/termio.h>
#include <lib/string.h>
#include <error.h>

#define KBD_DATA_PORT       0x60
#define KBD_CMD_PORT        0x64

#define READLINE_READY      0
#define READLINE_BLOCKED    1

static uint8_t line_status  = READLINE_READY;
static char    linebuf[MAX_READLINE_LENGTH];
static int     pos = 0;

/* KBDUS means US Keyboard Layout. This is a scancode table
*  used to layout a standard US keyboard. I have left some
*  comments in to give you an idea of what key is what, even
*  though I set it's array index to 0. You can change that to
*  whatever you want using a macro, if you wish!
*  Source: http://www.osdever.net/bkerndev/Docs/keyboard.htm
*  */
unsigned const char kbdus[128] =
{
    0,  27, '1', '2', '3', '4', '5', '6', '7', '8',	/* 9 */
  '9', '0', '-', '=', '\b',	/* Backspace */
  '\t',			/* Tab */
  'q', 'w', 'e', 'r',	/* 19 */
  't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',	/* Enter key */
    0,			/* 29   - Control */
  'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';',	/* 39 */
 '\'', '`',   0,		/* Left shift */
 '\\', 'z', 'x', 'c', 'v', 'b', 'n',			/* 49 */
  'm', ',', '.', '/',   0,				/* Right shift */
  '*',
    0,	/* Alt */
  ' ',	/* Space bar */
    0,	/* Caps lock */
    0,	/* 59 - F1 key ... > */
    0,   0,   0,   0,   0,   0,   0,   0,
    0,	/* < ... F10 */
    0,	/* 69 - Num lock*/
    0,	/* Scroll Lock */
    0,	/* Home key */
    0,	/* Up Arrow */
    0,	/* Page Up */
  '-',
    0,	/* Left Arrow */
    0,
    0,	/* Right Arrow */
  '+',
    0,	/* 79 - End key*/
    0,	/* Down Arrow */
    0,	/* Page Down */
    0,	/* Insert Key */
    0,	/* Delete Key */
    0,   0,   0,
    0,	/* F11 Key */
    0,	/* F12 Key */
    0,	/* All other keys are undefined */
};

void kbd_interrupt()
{
        unsigned char scancode;

        scancode = inb(KBD_DATA_PORT);
        if (!(scancode & 0x80)) {
                char last_char = kbdus[scancode];
                if (line_status == READLINE_BLOCKED) {
                        if (pos < MAX_READLINE_LENGTH-1) {
                                linebuf[pos++] = last_char;
                        }
                        line_status  = READLINE_READY;
                }
        }
}

uint8_t block_and_read_char()
{
        line_status = READLINE_BLOCKED;
        while (line_status == READLINE_BLOCKED)
                asm("hlt");

        if (pos > 0)
                return linebuf[--pos];

        return 0;
}

void keyboard_init()
{
        // Flush keyboard buffer
        while (inb(KBD_CMD_PORT) & 1)
                inb(KBD_DATA_PORT);

        // Activate keyboard
        outb(KBD_CMD_PORT, 0xae);
        io_wait();

        // Enable keyboard scanning
        outb(KBD_DATA_PORT, 0xf4);
        io_wait();
}
