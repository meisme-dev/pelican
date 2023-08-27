#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <terminal/psf.h>
#include <terminal/terminal.h>
#include <video/framebuffer.h>

#include <stdarg.h>
#include <terminal/log.h>

extern char _binary____assets_bold_psf_start[];
extern char _binary____assets_regular_psf_start[];

#define PADDING 0

struct limine_framebuffer *framebuffer;

static uint32_t x = 0, y = 0, fg = 0xffffff, bg = 0;
static _psf_font_t *psf_font;

bool terminal_init(void) {
  framebuffer = framebuffer_create();
  if (framebuffer == NULL) {
    return false;
  }
  set_bold(true);
  log(SUCCESS, "Initialized terminal");
  return true;
}

void set_bold(bool bold) {
  if (bold) {
    psf_font = (_psf_font_t *)&_binary____assets_bold_psf_start;
    psf_init((_psf_font_t *)&_binary____assets_bold_psf_start);
    return;
  }
  psf_font = (_psf_font_t *)&_binary____assets_regular_psf_start;
  psf_init((_psf_font_t *)&_binary____assets_regular_psf_start);
}

void reset_pos(void) {
  x = 0;
  y = 0;
}

void set_col(uint32_t nbg, uint32_t nfg) {
  bg = nbg;
  fg = nfg;
}

static void newline(void) {
  y += psf_font->height;
  x = 0;
}

void kputchar(const char c) {
  if (c == '\n') {
    newline();
    return;
  }
  psf_putchar(c, &x, &y, fg, bg);
  // x += 8;
  // y += 1;
}

void kputs(const char *c) {
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
      char str[65] = {' '};
      str[64] = '\0';
      ptr++;
      switch (*ptr++) {
        case 's':
          kputs(va_arg(args, char *));
          break;

        case 'd':
          itoa(va_arg(args, int64_t), str, 10);
          kputs(str);
          break;

        case 'u':
          utoa(va_arg(args, uint64_t), str, 10);
          kputs(str);
          break;

        case 'x':
          utoa(va_arg(args, uint64_t), str, 16);
          kputs(str);
          break;

        case 'b':
          utoa(va_arg(args, uint64_t), str, 2);
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
