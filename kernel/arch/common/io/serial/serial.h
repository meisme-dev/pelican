#pragma once

#include <stdbool.h>
#include <stdint.h>

#define COM1 0x3f8

bool serial_init(uint16_t port);
void serial_send(uint16_t port, const char c);
