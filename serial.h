#ifndef SERIAL_H
#define SERIAL_H

#define COM1  0x3F8
#define COM2	0x2F8
#define COM3	0x3E8
#define COM4	0x2E8

void init_serial();
int serial_received();
char read_serial();
int is_transmit_empty();
void write_serial(char a);

#endif
