#ifndef IO_H
#define IO_H
#include <stdint.h>
uint32_t inl(uint16_t port);
uint16_t inw(uint16_t port);
uint8_t inb(uint16_t port);
void outl(uint16_t port, uint32_t l);
void outw(uint16_t port, uint16_t w);
void outb(uint16_t port, uint8_t b);
#endif