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

#include "serial.h"
#include "io.h"

// Reference: https://wiki.osdev.org/Serial_Ports

static uint16_t port = SERIAL_BASE(CONFIG_SERIAL);

/*
 * Initialize the serial port writing kernel output at startup.
 */
void init_serial() {

  // Disable all interrupts
  outb(SERIAL_BASE(port), 0x00);

  // Enable DLAB to set baud rate divisor.
  // Line Control Register. The most significant bit of this register is the DLAB (Divisor
  // Latch Access Bit).
  outb(SERIAL_LINE_CONTROL_PORT(port), 0x80);

  // Set divisor to 1 (lo byte) for 115200 baud, 2 for 57600 and 3
  // for 38400. The next byte holds the most significant byte of the divisor.
  outb(SERIAL_BASE(port), BAUD_115200);
  outb(SERIAL_BASE(port) + 1, 0x00);

  outb(SERIAL_LINE_CONTROL_PORT(port), 0x03);  // 8 bits, no parity, one stop bit
  outb(SERIAL_FIFO_CONTROL_PORT(port), 0xC7);  // Enable FIFO, clear them, with 14-byte threshold
  outb(SERIAL_MODEM_CONTROL_PORT(port), 0x0B); // IRQs enabled, RTS/DSR set
}

/*
 * Check to see if data can be read from the serial port
 */
int serial_received() {
   return inb(SERIAL_LINE_STATUS_PORT(port)) & 1;
}

/*
 * Reads one character from the serial input buffer
 */
char read_serial() {
   while (serial_received() == 0);

   return inb(SERIAL_BASE(port));
}

/*
 * Checks to see if data is ready to be sent
 */
int is_transmit_empty() {
   return inb(SERIAL_LINE_STATUS_PORT(port)) & 0x20;
}

/*
 * Write one character to the serial port
 */
void write_serial(char a) {
   while (is_transmit_empty() == 0);

   outb(SERIAL_BASE(port),a);
}

#endif