#include "../libk/string.h"
#include "display/framebuffer.h"
#include "display/terminal.h"

extern unsigned char _binary_assets_font_sfn_start;

void kstart() {
    struct limine_framebuffer *framebuffer = create_fb();
    memset((uint32_t *)framebuffer->address, 0x3b4252, framebuffer->width * framebuffer->height);
    Buffer dst;
    dst.ptr = (uint8_t *)framebuffer->address;
    dst.bg = 0x3b4252;
    dst.fg = 0xa3be8c;
    dst.x = 16;
    dst.y = 16;
    dst.w = framebuffer->width;
    dst.h = framebuffer->height;
    dst.p = framebuffer->pitch;
    kputs(&_binary_assets_font_sfn_start, dst, "Welcome to Pelican");
    for(;;);
}