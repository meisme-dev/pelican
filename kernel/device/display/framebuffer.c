#include "framebuffer.h"
#include <stddef.h>

static volatile struct limine_framebuffer_request request = {
    .id = LIMINE_FRAMEBUFFER_REQUEST,
    .revision = 0
};

struct limine_framebuffer *create_fb() {
    if(request.response == NULL || request.response->framebuffer_count < 1) {
        return NULL;
    }
    return request.response->framebuffers[0];
}