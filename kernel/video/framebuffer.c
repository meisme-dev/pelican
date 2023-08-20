#include <video/framebuffer.h>
#include <stddef.h>

static volatile struct limine_framebuffer_request request = {
    .id = LIMINE_FRAMEBUFFER_REQUEST,
    .revision = 0};

struct limine_framebuffer *framebuffer_create(void) {
  if (request.response == NULL || request.response->framebuffer_count < 1) {
    return NULL;
  }
  return request.response->framebuffers[0];
}

void put_pixel(uint32_t x, uint32_t y, uint32_t c, struct limine_framebuffer *fb) {
  if ((*(uint64_t *)fb->address + x) + (y * fb->width) < *(uint64_t *)fb->address + (fb->height * fb->width)) {
    *(((uint32_t *)fb->address + x) + (y * fb->width)) = c;
  }
}
