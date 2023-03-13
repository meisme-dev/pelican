#include <stdarg.h>
#include <stddef.h>
#include "stdlib.h"
#include "../kernel/device/display/terminal.h"

#define putchar(x) kputchar(x)

void puts(char *str) {
    kputs(str);
    putchar('\n');
}

void printf(char *format, ...) {
    va_list ap;
    va_start(ap, format);
    char *ptr = format;
    while(*ptr) {
        if(*ptr == '%') {
            ptr++;
            switch(*ptr++) {
                case 's':
                    kputs(va_arg(ap, char *));
                    break;
                case 'd':
                    char str[10];
                    itoa(va_arg(ap, size_t), str);
                    kputs(str);
                    break;
            }
        } else {
            kputchar(*ptr++);
        }
    }
    va_end(ap);
}