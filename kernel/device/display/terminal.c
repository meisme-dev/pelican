#include <stddef.h>
#include "terminal.h"
#include "../../../libk/stdlib.h"
#include "../../../libk/string.h"
#define SSFN_CONSOLEBITMAP_TRUECOLOR
#include "../../../vendor/ssf/ssfn.h"

#include <stdarg.h>

#define PADDING 16

void set_terminal_font(char *src) {
    ssfn_src = (ssfn_font_t *)src;
}

void set_terminal_state(Buffer buffer) {
    ssfn_dst.ptr = buffer.ptr;
    ssfn_dst.bg = buffer.bg;
    ssfn_dst.fg = buffer.fg;
    ssfn_dst.x = buffer.x;
    ssfn_dst.y = buffer.y;
    ssfn_dst.w = buffer.w;
    ssfn_dst.h = buffer.h;
    ssfn_dst.p = buffer.p;
}

static void newline() {
    ssfn_dst.x = PADDING;
    ssfn_dst.y += ssfn_dst.p / ssfn_dst.h + (PADDING / 2);
}

void kputchar(const char c) {
    ssfn_putc(c);
}

void kputs(const char *c) {
    while(*c != '\0') {
        switch(*c) {
            case '\n':
                newline();
                break;
            default:
                kputchar(*c);
                break;
        }
        c++;
    }
}
