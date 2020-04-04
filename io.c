#include "io.h"
#include "types.h"

void outb(__uint16_t port, __uint8_t val) {
    asm volatile("outb %0, %1" : : "a"(val), "Nd"(port));
}

unsigned char inb(__uint16_t port) {
    __uint8_t ret;
    asm volatile("inb %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}
