#include "sys/drivers/vga/vga.h"
#include "sys/drivers/vga/colors.h"
#include "util/ints.h"

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
        char data[255] = {'\0'};
        itoa_32(i, data);
        print_info.data = "Hello";
        print_info.bg_color = BLACK;
        print_info.fg_color = GRAY + BRIGHT;
        print_info.x = 1;
        print_info.y = 25;
        kprint(print_info);
        sleep(0xFFFFFFF);
    }    
}
