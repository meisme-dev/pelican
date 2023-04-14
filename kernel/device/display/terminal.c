#include <device/display/framebuffer.h>
#include <stddef.h>
#include <device/display/terminal.h>
#include <stdlib.h>
#include <string.h>
#define SSFN_CONSOLEBITMAP_TRUECOLOR
#include <ssf/ssfn.h>

#include <stdarg.h>

#define PADDING 32

extern unsigned char _binary_font_sfn_start;
bool term_failed = true;


bool init_terminal(void) {
    struct limine_framebuffer *framebuffer = create_fb();
    if(framebuffer == NULL) {
        return false;
    }
    ssfn_dst.ptr = (uint8_t *)framebuffer->address;
    ssfn_dst.bg = 0x000000;
    ssfn_dst.fg = 0xffffff;
    ssfn_dst.x = PADDING;
    ssfn_dst.y = PADDING;
    ssfn_dst.w = framebuffer->width;
    ssfn_dst.h = framebuffer->height;
    ssfn_dst.p = framebuffer->pitch;
    ssfn_src = (ssfn_font_t *)&_binary_font_sfn_start;
    term_failed = false;
    trace();
    return true;
}

static void newline(void) {
    ssfn_dst.x = PADDING;
    ssfn_dst.y += ssfn_dst.p / ssfn_dst.h + (PADDING / 2);
}

void kputchar(const char c) {
    if(term_failed) {
        return;
    }
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
            char str[20] = {' '};
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

void _trace(const char *file, size_t line) {
    printf("At %s:%d:\n", file, line);
}
