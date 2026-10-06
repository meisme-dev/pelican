#include "psf.h"
#include <stddef.h>
#include <stdint.h>
#include <sync/lock.h>
#include <terminal/terminal.h>
#include <video/framebuffer.h>

static psf_font_t *psf_font;
static uint16_t font_table[65536] = {0};
static bool unicode = false;

void psf_init(psf_font_t *font, psf_font_t *end) {
  if (font == NULL || font->magic != PSF_FONT_MAGIC) {
    return;
  }

  uint16_t glyph = 0;
  psf_font = font;

  if (font->flags == 0) {
    unicode = false;
    return;
  }

  unsigned char *s = (unsigned char *)psf_font + psf_font->header_size + psf_font->count * psf_font->bpg;
  while (s < (unsigned char *)end) {
    uint16_t uc = (uint16_t)s[0];
    if (uc == 0xFF) {
      glyph++;
      s++;
      continue;
    } else if (uc & 128) {
      if ((uc & 32) == 0) {
        uc = ((s[0] & 0x1F) << 6) + (s[1] & 0x3F);
        s++;
      } else if ((uc & 16) == 0) {
        uc = ((((s[0] & 0xF) << 6) + (s[1] & 0x3F)) << 6) + (s[2] & 0x3F);
        s += 2;
      } else if ((uc & 8) == 0) {
        uc = ((((((s[0] & 0x7) << 6) + (s[1] & 0x3F)) << 6) + (s[2] & 0x3F)) << 6) + (s[3] & 0x3F);
        s += 3;
      } else
        uc = 0;
    }

    font_table[uc] = glyph;
    s++;
  }

  unicode = true;
}

void psf_putchar(uint16_t character, uint32_t *cx, uint32_t *cy, uint32_t fg, uint32_t bg) {
  static atomic_flag lock = ATOMIC_FLAG_INIT;
  acquire(&lock);
  uint32_t x = *cx, y = *cy;
  uint16_t c = character;

  if (unicode == true) {
    c = font_table[c];
  }

  unsigned char *glyph = (unsigned char *)psf_font + psf_font->header_size + (c > 0 && c < psf_font->count ? c : 0) * psf_font->bpg;

  for (uint32_t i = 0; i < psf_font->height; i++, y++) {
    for (uint32_t j = psf_font->width; j--; x++) {
      if ((glyph[i] >> j) & 1) {
        put_pixel(x, y, fg, framebuffer);
      } else {
        put_pixel(x, y, bg, framebuffer);
      }
    }

    x = *cx;
  }

  *cx += psf_font->width;
  if (*cx >= framebuffer->width) {
    *cy += psf_font->height;
    *cx = 0;
  }
  release(&lock);
}
