#include "arch/i386/display/vga/vga.h"
#include "arch/i386/display/vga/colors.h"
#include "arch/i386/err/exception.h"
#include "util/ints.h"
#include "util/strings.h"

void sleep(uint32_t length) {
  for(;;) {
    asm volatile("nop");
    length--;
    if(length <= 0)
      break;
    }
}

void kstart() {
    uint8_t i = 0;
    for(;;) {
        i++;
        PrintInfo print_info;
        print_info.data = "Welcome";
        print_info.bg_color = BLACK;
        print_info.fg_color = GRAY + BRIGHT;
        print_info.x = 0;
        print_info.y = 0;
        kprint(print_info);
        sleep(0xFFFFFFF);
        panic("Test panic");
    }    
}
