/**
 * SPDX-FileCopyrightText: 2020 Anuradha Weeraman <anuradha@weeraman.com>
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include <shell.h>
#include <keyboard.h>
#include <libk.h>

char line[MAX_READLINE_LENGTH];

const char* commands = "Available commands: help, exit";

static int cmd(const char *cmd, char *input)
{
        if (strncmp (cmd, input, MAX_READLINE_LENGTH) == 0)
                return 0;

        return 1;
}

void start_interactive_shell()
{
        printf("\nMinos version %s\n", CONFIG_VERSION);

#if CONFIG_DRV_KEYBOARD
        for (;;) {
                printf("# ");
                block_and_readline(line);

                if  (cmd("", line) == 0) {
                        // do nothing
                } else if (cmd("help", line) == 0) {
                        printf ("%s\n", commands);
                } else if (cmd("exit", line) == 0) {
                        break;
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
