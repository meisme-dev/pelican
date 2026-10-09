#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <sync/lock.h>
#include <terminal/log.h>
#include <terminal/psf.h>
#include <terminal/terminal.h>
#include <video/framebuffer.h>

extern char _binary____assets_bold_psf_start[];
extern char _binary____assets_bold_psf_end[];
extern char _binary____assets_regular_psf_start[];
extern char _binary____assets_regular_psf_end[];
extern uint32_t _binary____assets_bg_rgb_start[];
extern uint32_t _binary____assets_bg_rgb_end[];

#define PADDING 0
#define BOLD false
#define FG 0xbababa
#define BG 0x000000

#define BG_WIDTH 960.0
#define BG_HEIGHT 540.0

struct limine_framebuffer *framebuffer;

static uint32_t x = 0, y = 0, fg = FG, bg = BG;
static psf_font_t *psf_font;
static atomic_flag locks[2] = {ATOMIC_FLAG_INIT};

bool terminal_init(void) {
  framebuffer = framebuffer_create();

  if (framebuffer == NULL) {
    return false;
  }

  set_bold(BOLD);
  log_print(OK, "Initialized terminal");
  return true;
}

void reset_format(void) {
  acquire(&locks[1]);
  set_color(BG, FG);
  set_bold(BOLD);
  release(&locks[1]);
}

void get_defaults(uint32_t *nbg, uint32_t *nfg, bool *nb) {
  if (nbg != NULL) {
    *nbg = BG;
  }

  if (nfg != NULL) {
    *nfg = FG;
  }

  if (nb != NULL) {
    *nb = BOLD;
  }
}

void terminal_scroll(uint32_t n) {
  static atomic_flag lock = ATOMIC_FLAG_INIT;
  acquire(&lock);
  memmove(framebuffer->address, (uint32_t *)framebuffer->address + framebuffer->width * n, (framebuffer->width * framebuffer->height - framebuffer->width * n) * (framebuffer->bpp / 8));
  for (uint32_t i = 0; i < n; i++) {
    for (uint32_t j = 0; j < framebuffer->width; j++) {
      put_pixel(j, framebuffer->height - i, bg, framebuffer);
    }
  }
  release(&lock);
}

void set_bold(bool bold) {
  acquire(&locks[0]);
  if (bold) {
    psf_font = (psf_font_t *)&_binary____assets_bold_psf_start;
    psf_init((psf_font_t *)&_binary____assets_bold_psf_start, (psf_font_t *)&_binary____assets_bold_psf_end);
    release(&locks[0]);
    return;
  }
  psf_font = (psf_font_t *)&_binary____assets_regular_psf_start;
  psf_init((psf_font_t *)&_binary____assets_regular_psf_start, (psf_font_t *)&_binary____assets_regular_psf_end);
  release(&locks[0]);
}

void reset_pos(void) {
  acquire(&locks[0]);
  x = 0;
  y = 0;
  release(&locks[0]);
}

void set_color(uint32_t nbg, uint32_t nfg) {
  acquire(&locks[0]);
  bg = nbg;
  fg = nfg;
  release(&locks[0]);
}

void kputchar(const char c) {
  static atomic_flag lock = ATOMIC_FLAG_INIT;
  acquire(&lock);
  if (c == '\n') {
    if (y >= framebuffer->height - psf_font->height) {
      y = framebuffer->height - psf_font->height;
      terminal_scroll(psf_font->height);
    } else {
      y += psf_font->height;
    }
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

        case 'i':
          itoa(va_arg(args, int32_t), str, 10);
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

#undef PADDING
#undef BG
#undef FG
#undef BOLD