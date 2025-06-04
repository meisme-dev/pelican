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

static uint32_t x = 0, y = 0, fg = 0xffffff, bg = 0;
static psf_font_t *psf_font;
static atomic_flag locks[4] = {ATOMIC_FLAG_INIT};

bool terminal_init(void) {
  framebuffer = framebuffer_create();
  if (framebuffer == NULL) {
    return false;
  }
  set_bold(false);
  log_print(SUCCESS, "Initialized terminal");
  return true;
}

void set_bold(bool bold) {
  acquire(&locks[0]);
  if (bold) {
    psf_font = (psf_font_t *)&_binary____assets_bold_psf_start;
    psf_init((psf_font_t *)&_binary____assets_bold_psf_start);
    release(&locks[0]);
    return;
  }
  psf_font = (psf_font_t *)&_binary____assets_regular_psf_start;
  psf_init((psf_font_t *)&_binary____assets_regular_psf_start);
  release(&locks[0]);
}

void reset_pos(void) {
  acquire(&locks[0]);
  x = 0;
  y = 0;
  release(&locks[0]);
}

void set_col(uint32_t nbg, uint32_t nfg) {
  acquire(&locks[0]);
  bg = nbg;
  fg = nfg;
  release(&locks[0]);
}

void kputchar(const char c) {
  acquire(&locks[3]);
  if (c == '\n') {
    y += psf_font->height;
    x = 0;
    release(&locks[3]);
    return;
  }
  psf_putchar(c, &x, &y, fg, bg);
  release(&locks[3]);
}

void kputs(const char *c) {
  acquire(&locks[2]);
  while (*c != '\0') {
    kputchar(*c);
    c++;
  }
  release(&locks[2]);
}

void puts(const char *str) {
  acquire(&locks[0]);
  kputs(str);
  kputchar('\n');
  release(&locks[0]);
}

void printf(char *format, ...) {
  acquire(&locks[0]);
  va_list args;
  va_start(args, format);
  vprintf(format, args);
  va_end(args);
  release(&locks[0]);
}

void vprintf(char *format, va_list args) {
  acquire(&locks[1]);
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
  release(&locks[1]);
}

void _trace(const char *file, size_t line) {
  acquire(&locks[0]);
  printf("At %s:%d:\n", file, line);
  release(&locks[0]);
}
