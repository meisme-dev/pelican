#include "io.h"

uint32_t inl(uint16_t port) {
  uint32_t data;
  asm volatile("inl %1, %0"
               : "=a"(data)
               : "Nd"(port));
  return data;
}

uint16_t inw(uint16_t port) {
  uint16_t data;
  asm volatile("inw %1, %0"
               : "=a"(data)
               : "Nd"(port));
  return data;
}

uint8_t inb(uint16_t port) {
  uint8_t data;
  asm volatile("inb %1, %0"
               : "=a"(data)
               : "Nd"(port));
  return data;
}

void outl(uint16_t port, uint32_t value) {
  asm volatile("outl %0, %1" ::"a"(value), "Nd"(port)
               : "memory");
}

void outw(uint16_t port, uint16_t value) {
  asm volatile("outw %0, %1" ::"a"(value), "Nd"(port)
               : "memory");
}

void outb(uint16_t port, uint8_t value) {
  asm volatile("outb %0, %1" ::"a"(value), "Nd"(port)
               : "memory");
}
