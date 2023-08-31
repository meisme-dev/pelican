#include <stdarg.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <sync/lock.h>
#include <terminal/log.h>
#include <terminal/psf.h>
#include <terminal/terminal.h>
#include <video/framebuffer.h>

extern char _binary____assets_bold_psf_start[];
extern char _binary____assets_regular_psf_start[];

#define PADDING 0

struct limine_framebuffer *framebuffer;
static atomic_flag global_lock = ATOMIC_FLAG_INIT;

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
  static atomic_flag lock = ATOMIC_FLAG_INIT;
  acquire(&lock);
  if (bold) {
    psf_font = (_psf_font_t *)&_binary____assets_bold_psf_start;
    psf_init((_psf_font_t *)&_binary____assets_bold_psf_start);
    release(&lock);
    return;
  }
  psf_font = (_psf_font_t *)&_binary____assets_regular_psf_start;
  psf_init((_psf_font_t *)&_binary____assets_regular_psf_start);
  release(&lock);
}

inline void reset_pos(void) {
  static atomic_flag lock = ATOMIC_FLAG_INIT;
  acquire(&lock);
  x = 0;
  y = 0;
  release(&lock);
}

inline void set_col(uint32_t nbg, uint32_t nfg) {
  static atomic_flag lock = ATOMIC_FLAG_INIT;
  acquire(&lock);
  bg = nbg;
  fg = nfg;
  release(&lock);
}

void kputchar(const char c) {
  static atomic_flag lock = ATOMIC_FLAG_INIT;
  acquire(&lock);
  if (c == '\n') {
    y += psf_font->height;
    x = 0;
    release(&lock);
    return;
  }
  psf_putchar(c, &x, &y, fg, bg);
  release(&lock);
}

void kputs(const char *c) {
  static atomic_flag lock = ATOMIC_FLAG_INIT;
  acquire(&lock);
  while (*c != '\0') {
    kputchar(*c);
    c++;
  }
  release(&lock);
}

void puts(const char *str) {
  static atomic_flag lock = ATOMIC_FLAG_INIT;
  acquire(&lock);
  kputs(str);
  kputchar('\n');
  release(&lock);
}

void printf(char *format, ...) {
  static atomic_flag lock = ATOMIC_FLAG_INIT;
  acquire(&lock);
  va_list args;
  va_start(args, format);
  vprintf(format, args);
  va_end(args);
  release(&lock);
}

void vprintf(char *format, va_list args) {
  static atomic_flag lock = ATOMIC_FLAG_INIT;
  acquire(&lock);
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
  release(&lock);
}

void _trace(const char *file, size_t line) {
  printf("At %s:%d:\n", file, line);
}
