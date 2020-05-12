/**
 * SPDX-FileCopyrightText: 2020 Anuradha Weeraman <anuradha@weeraman.com>
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include <cmd/shell.h>
#include <sys/keyboard.h>
#include <lib/termio.h>
#include <lib/string.h>
#include <ppm/odyssey.h>
#include <sys/tty.h>

char line[MAX_READLINE_LENGTH];

const char* commands = "Available commands: clear, help, splash, exit";

static void splash()
{
        clear_screen();
#ifdef CONFIG_FRAMEBUFFER_RGB
        for (int prows = 0; prows < PPM_ROWS; prows++) {
                for (int pcols = 0; pcols < PPM_COLS; pcols++) {
                        uint32_t pos = ((prows * pcols) + pcols) * 3;
                        char r = ppm_array[pos];
                        char g = ppm_array[pos+1];
                        char b = ppm_array[pos+2];
                        int ri = (r << 16) & 0xff0000;
                        int gi = (g << 8) & 0xff00;
                        int bi = b & 0xff;
                        uint32_t ci = 0x00000000 | ri | gi | bi;
                        draw_pixel(prows, pcols, ci);
                }
        }
#endif
}

static int cmd(const char *cmd, char *input)
{
        if (strncmp (cmd, input, MAX_READLINE_LENGTH) == 0)
                return 0;

        return 1;
}

void start_interactive_shell()
{
        printf("\nOdyssey v%s\n",
                        STRINGIFY(CONFIG_VERSION_MAJOR) "." \
                        STRINGIFY(CONFIG_VERSION_MINOR));

#ifdef CONFIG_KEYBOARD
        for (;;) {
                printf("# ");
                block_and_readline(line);

                if  (cmd("", line) == 0) {
                        // do nothing
                } else if (cmd("help", line) == 0) {
                        printf ("%s\n", commands);
                } else if (cmd("exit", line) == 0) {
                        break;
                } else if (cmd("clear", line) == 0) {
                        clear_screen();
                } else if (cmd("splash", line) == 0) {
                        splash();
                } else {
                        printf ("Unknown command. %s\n", commands);
                }
        }
#else
        printf("Enable keyboard driver for interactive shell\n");
        for (;;) {
                asm("hlt");
        }
#endif
}
