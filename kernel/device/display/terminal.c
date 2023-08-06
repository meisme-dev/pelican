#include <device/display/framebuffer.h>
#include <device/display/terminal.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#define SSFN_CONSOLEBITMAP_TRUECOLOR
#include <ssf/ssfn.h>

#include <stdarg.h>

#define PADDING 0

extern unsigned char _binary_font_sfn_start;
bool term_failed = true;
struct limine_framebuffer *framebuffer;

bool terminal_init(void) {
  framebuffer = framebuffer_create();
  if (framebuffer == NULL) {
    return false;
  }
  ssfn_dst.ptr = (uint8_t *)framebuffer->address;
  ssfn_dst.bg = 0x000000;
  ssfn_dst.fg = 0xffffff;
  ssfn_dst.x = PADDING;
  ssfn_dst.y = PADDING;
  ssfn_dst.w = framebuffer->width;
  ssfn_dst.h = framebuffer->height;
  ssfn_dst.p = framebuffer->pitch;
  ssfn_src = (ssfn_font_t *)&_binary_font_sfn_start;
  term_failed = false;
  return true;
}

void reset_pos(void) {
  ssfn_dst.x = PADDING;
  ssfn_dst.y = PADDING;
}

void set_col(uint32_t bg, uint32_t fg) {
  ssfn_dst.bg = bg;
  ssfn_dst.fg = fg;
}

static void newline(void) {
  ssfn_dst.x = PADDING;
  ssfn_dst.y += ssfn_src->height;
}

void kputchar(const char c) {
  if (term_failed) {
    return;
  }
  if (c == '\n') {
    newline();
    return;
  }
  ssfn_putc(c);
}

static void kputs(const char *c) {
  while (*c != '\0') {
    kputchar(*c);
    c++;
  }
}

void puts(const char *str) {
  kputs(str);
  kputchar('\n');
}

void printf(char *format, ...) {
  va_list args;
  va_start(args, format);
  vprintf(format, args);
  va_end(args);
}

void vprintf(char *format, va_list args) {
  char *ptr = format;
  while (*ptr) {
    if (*ptr == '%') {
      char str[20] = {' '};
      str[19] = '\0';
      ptr++;
      switch (*ptr++) {
        case 's':
          kputs(va_arg(args, char *));
          break;

        case 'd':
          itoa(va_arg(args, int64_t), str);
          kputs(str);
          break;

        case 'u':
          itoa(va_arg(args, uint64_t), str);
          kputs(str);
          break;
      }
    } else {
      kputchar(*ptr++);
    }
  }
}

void _trace(const char *file, size_t line) {
  printf("At %s:%d:\n", file, line);
}
