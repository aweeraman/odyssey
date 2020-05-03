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
 *
 * KBDUS scancode table, Copyright Brandon F. (friesenb@gmail.com)
 */

#include "keyboard.h"
#include "io.h"
#include "libk.h"

static char    linebuf[MAX_READLINE_LENGTH];
static int     line_counter = 0;
static uint8_t line_status  = READLINE_READY;

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

void kbd_interrupt() {
        unsigned char scancode;

        scancode = inb(0x60);
        if (!(scancode & 0x80)) {
                char last_char = kbdus[scancode];
                if (line_status == READLINE_BLOCKED) {
                        if (line_counter < MAX_READLINE_LENGTH-1) {
                                if (last_char == '\n') {
                                        linebuf[line_counter] = '\0';
                                        line_counter = 0;
                                        line_status  = READLINE_READY;
                                } else {
                                        linebuf[line_counter] = last_char;
                                        line_counter = line_counter + 1;
                                }
                                putchar(last_char);
                        } else {
                                putchar('\n');
                                linebuf[line_counter] = '\0';
                                line_counter = 0;
                                line_status  = READLINE_READY;
                        }
                }
        }
}

uint8_t block_and_readline(char *line) {
        if (line_status == READLINE_BLOCKED)
                return EINUSE;

        line_status = READLINE_BLOCKED;
        while (line_status == READLINE_BLOCKED)
                asm("hlt");

        strncpy(line, linebuf, MAX_READLINE_LENGTH);
        return strnlen(line, MAX_READLINE_LENGTH);
}
