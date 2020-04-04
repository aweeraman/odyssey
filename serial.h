#ifndef SERIAL_H
#define SERIAL_H

#include "types.h"

#define COM1  0x3F8
#define COM2	0x2F8
#define COM3	0x3E8
#define COM4	0x2E8

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
