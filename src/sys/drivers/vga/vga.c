#include "vga.h"

void kprint(PrintInfo print_info) {
    volatile char *vmem = (volatile char*)0xB8000 + (print_info.x * 160 + (print_info.y * 2) / 2 * 2);
    while(*print_info.data != 0) {
        *vmem++ = *print_info.data++;
        *vmem++ = (print_info.bg_color << 4) | (print_info.fg_color);
    }
}