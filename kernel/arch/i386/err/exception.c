#include "../display/vga/vga.h"
#include "../display/vga/colors.h"
#include "exception.h"
#include "../../../util/strings.h"
#include "../../../util/ints.h"

void _panic(const char *msg, const char *file, int line) {
    char message[1024] = {"Kernel panic at "};
    char line_str[255] = {"\0"};
    itoa_32((uint32_t)line, line_str);
    char *tmp1 = strcat(message, file);
    char *tmp2 = strcat(message, ", line ");
    char *tmp3 = strcat(message, line_str);    
    char *tmp4 = strcat(message, ". Message: ");
    char *tmp5 = strcat(message, msg);

    PrintInfo print_info;
    print_info.bg_color = RED;
    print_info.fg_color = GRAY + BRIGHT;
    print_info.data = tmp5;
    print_info.x = 0;
    print_info.y = 0;
    kprint(print_info);
    asm("hlt");
}