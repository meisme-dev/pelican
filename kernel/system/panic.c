#include <device/display/terminal.h>
#include <string.h>
#include <system/panic.h>

void _panic(const char *file, size_t line, const char *msg) {
  for (uint32_t i = 0; i < framebuffer->height * framebuffer->width; i++) {
    ((uint32_t *)(framebuffer->address))[i] = 0xff0000;
  }
  reset_pos();
  puts("BEGIN KERNEL PANIC");
  puts("--------------------------------");
  printf("AT %s:%d:\n", file, line);
  printf("%s\n", msg);
  puts("--------------------------------");
  puts("END KERNEL PANIC");
  for (;;) {
    asm __volatile__("cli");
  }
}