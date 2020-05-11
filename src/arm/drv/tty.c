/**
 * SPDX-FileCopyrightText: 2020 Anuradha Weeraman <anuradha@weeraman.com>
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include <sys/tty.h>

#ifdef CONFIG_SERIAL
#include <sys/serial.h>
#endif

void printc(uint8_t ch)
{
#ifdef CONFIG_SERIAL
        write_serial(ch);
#endif
}
