#include <stddef.h>
#include "terminal.h"
#include "../../../libk/stdlib.h"
#include "../../../libk/string.h"
#define SSFN_CONSOLEBITMAP_TRUECOLOR
#include "../../../vendor/ssf/ssfn.h"

#include <stdarg.h>

#define PADDING 32

void set_terminal_font(unsigned char *src) {
    ssfn_src = (ssfn_font_t *)src;
}

void set_terminal_state(TerminalInfo terminal_info) {
    ssfn_dst.ptr = terminal_info.ptr;
    ssfn_dst.bg = terminal_info.bg;
    ssfn_dst.fg = terminal_info.fg;
    ssfn_dst.x = terminal_info.x;
    ssfn_dst.y = terminal_info.y;
    ssfn_dst.w = terminal_info.w;
    ssfn_dst.h = terminal_info.h;
    ssfn_dst.p = terminal_info.p;
}

static void newline() {
    ssfn_dst.x = PADDING;
    ssfn_dst.y += ssfn_dst.p / ssfn_dst.h + (PADDING / 2);
}

static void kputchar(const char c) {
    if(c == '\n') {
        newline();
        return;
    }
    ssfn_putc(c);
}

static void kputs(const char *c) {
    while(*c != '\0') {
        kputchar(*c);
        c++;
    }
}

void puts(char *str) {
    kputs(str);
    kputchar('\n');
}

void printf(char *format, ...) {
    va_list ap;
    va_start(ap, format);
    char *ptr = format;
    while(*ptr) {
        if(*ptr == '%') {
            char str[256] = {' '};
            ptr++;
            switch(*ptr++) {
                case 's':
                    kputs(va_arg(ap, char *));
                    break;
                case 'd':
                    itoa(va_arg(ap, int64_t), str);
                    kputs(str);
                    break;

                case 'u':
                    itoa(va_arg(ap, uint64_t), str);
                    kputs(str);
                    break;
            }
        } else {
            kputchar(*ptr++);
        }
    }
    va_end(ap);
}