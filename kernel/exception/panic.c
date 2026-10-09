#include "panic.h"
#include <arch/common/cpu/cpu.h>
#include <stdatomic.h>
#include <sync/lock.h>
#include <terminal/terminal.h>
#include <video/framebuffer.h>

void _panic(const char *file, size_t line, char *format, ...) {
  static atomic_flag lock = ATOMIC_FLAG_INIT; /* Never release lock */
  acquire(&lock);

  va_list args;

  for (uint32_t x = 0; x < framebuffer->width; x++) {
    for (uint32_t y = 0; y < framebuffer->height; y++) {
      if ((*(uintptr_t *)framebuffer->address + x) + (y * framebuffer->width) < *(uintptr_t *)framebuffer->address + (framebuffer->height * framebuffer->width) && *(((uint32_t *)framebuffer->address + x) + (y * framebuffer->width)) > 0) {
        uint32_t *pixel = (((uint32_t *)framebuffer->address + x) + (y * framebuffer->width));
        *pixel = 0xff0000;
      }
    }
  }

  va_start(args, format);
  set_bold(true);
  reset_pos();
  set_color(0xff0000, 0xffffff);

  printf("KERNEL PANIC FROM %s:%d:\n", file, line);
  vprintf(format, args);

  set_bold(false);
  puts("");
  va_end(args);

  cpu_halt();
}
