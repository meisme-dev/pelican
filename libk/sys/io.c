#include "io.h"

uint32_t inl(uint16_t port) {
    uint32_t data;
    asm("inl %%dx, %0" : "=r"(data) : "r"(port));
    return data;
}

uint16_t inw(uint16_t port) {
    uint16_t data;
    asm("inw %%dx, %0" : "=r"(data) : "r"(port));
    return data;
}

uint8_t inb(uint16_t port) {
    uint8_t data;
    asm("inb %%dx, %0" : "=r"(data) : "r"(port));
    return data;
}

void outl(uint16_t port, uint32_t l) {
    asm("outl %0, %1" :: "r"(l), "r"(port));
}

void outw(uint16_t port, uint16_t w) {
    asm("outw %0, %1" :: "r"(w), "r"(port));
}

void outb(uint16_t port, uint8_t b) {
    asm("outb %0, %1" :: "r"(b), "r"(port));
}