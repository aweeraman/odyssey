/**
 * SPDX-FileCopyrightText: 2020 Anuradha Weeraman <anuradha@weeraman.com>
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include <sys/panic.h>
#include <lib/termio.h>

void panic(char *str)
{
        printf("PANIC: %s\n", str);
        while(1) {}
}
