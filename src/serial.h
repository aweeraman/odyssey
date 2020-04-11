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
 */

#ifdef CONFIG_SERIAL

#ifndef SERIAL_H
#define SERIAL_H

#define COM1  0x3F8
#define COM2  0x2F8
#define COM3  0x3E8
#define COM4  0x2E8

#define SERIAL_BASE(base)               (base)
#define SERIAL_FIFO_CONTROL_PORT(base)  (base + 2)
#define SERIAL_LINE_CONTROL_PORT(base)  (base + 3)
#define SERIAL_MODEM_CONTROL_PORT(base) (base + 4)
#define SERIAL_LINE_STATUS_PORT(base)   (base + 5)
#define SERIAL_MODEM_STATUS_PORT(base)  (base + 6)

#define BAUD_115200 1
#define BAUD_57600  2
#define BAUD_38400  3

void init_serial();
int serial_received();
char read_serial();
int is_transmit_empty();
void write_serial(char a);

#endif

#endif
