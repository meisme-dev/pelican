#ifndef INTS_H
#define INTS_H
#include <stdint.h>
uint32_t intcat_32(uint32_t first, uint32_t second);
uint8_t intlen_32(uint32_t x);
void itoa_32(uint32_t x, char result[255]);
#endif