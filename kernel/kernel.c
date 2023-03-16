#include "../libk/string.h"
#include "../libk/stdlib.h"
#include "device/display/framebuffer.h"
#include "device/display/terminal.h"
#include "device/pci/pci.h"
#include "memory/pmm.h"

extern unsigned char _binary_assets_font_sfn_start;

void kstart() {
    struct limine_framebuffer *framebuffer = create_fb();
    memset((uint32_t *)framebuffer->address, 0x212121, framebuffer->width * framebuffer->height);
    TerminalInfo terminal_info;
    terminal_info.ptr = (uint8_t *)framebuffer->address;
    terminal_info.bg = 0x212121;
    terminal_info.fg = 0xa9fbff;
    terminal_info.x = 32;
    terminal_info.y = 32;
    terminal_info.w = framebuffer->width;
    terminal_info.h = framebuffer->height;
    terminal_info.p = framebuffer->pitch;
    set_terminal_state(terminal_info);
    set_terminal_font(&_binary_assets_font_sfn_start);
    init_pmm();
    for(;;);
} 