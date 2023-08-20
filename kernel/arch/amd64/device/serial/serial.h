#ifndef SERIAL_H
#define SERIAL_H

#include <stdbool.h>
#include <stdint.h>
#include <sys/io.h>

#define COM1 0x3f8

bool serial_init(uint16_t port);
void serial_send(uint16_t port, const char c);

#endif
