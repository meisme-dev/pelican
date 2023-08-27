#include "psf.h"
#include <stdlib.h>
#include <terminal/terminal.h>
#include <video/framebuffer.h>

static _psf_font_t *psf_font;

void psf_init(_psf_font_t *font) {
  psf_font = font;
}

void psf_putchar(uint16_t character, uint32_t *cx, uint32_t *cy, uint32_t fg, uint32_t bg) {
  uint32_t x = *cx, y = *cy;
  unsigned char *glyph = (unsigned char *)psf_font + psf_font->header_size + (character > 0 && character < psf_font->count ? character : 0) * psf_font->bpg;
  for (uint32_t i = 0; i < psf_font->height; i++, y++) {
    for (uint32_t j = psf_font->width; j--; x++) {
      if ((glyph[i] >> j) & 1) {
        put_pixel(x, y, fg, framebuffer);
      } else if (bg != 0) {
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
}
