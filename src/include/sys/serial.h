/**
 * SPDX-FileCopyrightText: 2020 Anuradha Weeraman <anuradha@weeraman.com>
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifdef CONFIG_SERIAL

#ifndef SERIAL_H
#define SERIAL_H

void            init_serial             ();
int             serial_received         ();
unsigned char   read_serial             ();
int             is_transmit_empty       ();
void            write_serial            (unsigned char a);

#endif

#endif
