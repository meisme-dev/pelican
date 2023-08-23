#include "panic.h"
#include <string.h>
#include <terminal/terminal.h>

void _panic(const char *file, size_t line, char *format, ...) {
  va_list args;
  uint8_t j = 0, e = 0;
  for (uint32_t i = 0; i < framebuffer->height * framebuffer->width; i++) {
    ((uint32_t *)(framebuffer->address))[i] = 0x0;
  }
  va_start(args, format);
  reset_pos();
  printf("PANICKED AT %s:%d:\n", file, line);
  vprintf(format, args);
  va_end(args);
  while (1) {
    e++;
    j++;
    e += e == 0;
    for (uint32_t i = 0; i < framebuffer->height * framebuffer->width; i++) {
      if (((uint32_t *)(framebuffer->address))[i] != 0xffffff) {
        ((uint32_t *)(framebuffer->address))[i] += ((j + i) % e) << 16;
      }
    }

    for (uint32_t i = 0; i < 0x3ffffff; i++) {
      asm volatile("nop");
    }
  }
}