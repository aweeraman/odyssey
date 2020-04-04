#ifndef IO_H
#define IO_H

#include "types.h"

void outb(__uint16_t port, __uint8_t val);
__uint8_t inb(__uint16_t port);

#endif
