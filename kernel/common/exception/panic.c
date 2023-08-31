#include "panic.h"
#include <common/cpu/cpu.h>
#include <string.h>
#include <terminal/terminal.h>
#include <video/framebuffer.h>

void _panic(const char *file, size_t line, char *format, ...) {
  va_list args;
  for (uint32_t i = 0; i < framebuffer->height * framebuffer->width; i++) {
    if (((uint32_t *)(framebuffer->address))[i] == 0x0) {
      ((uint32_t *)(framebuffer->address))[i] = 0xff0000;
    }
  }
  va_start(args, format);
  printf("KERNEL PANIC AT %s:%d:\n", file, line);
  vprintf(format, args);
  puts("");
  va_end(args);
  cpu_halt();
}