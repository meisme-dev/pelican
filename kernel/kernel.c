#include "../libk/string.h"
#include "../libk/stdlib.h"
#include "device/display/framebuffer.h"
#include "device/display/terminal.h"
#include "device/pci/pci.h"
#include "device/pci/vendors.h"


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
    kputs(&_binary_assets_font_sfn_start, dst, "Hello");
    for(;;);
} 