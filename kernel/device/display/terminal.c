#include <stddef.h>
#include "terminal.h"
#include "../../../libk/stdlib.h"
#include "../../../libk/string.h"
#define SSFN_CONSOLEBITMAP_TRUECOLOR
#include "../../../vendor/ssf/ssfn.h"


void kputs(char *src, Buffer dst, const char *c) {
    ssfn_src = (ssfn_font_t *)src;
    ssfn_dst.ptr = dst.ptr;
    ssfn_dst.bg = dst.bg;
    ssfn_dst.fg = dst.fg;
    ssfn_dst.x = dst.x;
    ssfn_dst.y = dst.y;
    ssfn_dst.w = dst.w;
    ssfn_dst.h = dst.h;
    ssfn_dst.p = dst.p;
    while(*c != '\0') {
        ssfn_putc(*c);
        c++;
    }
}