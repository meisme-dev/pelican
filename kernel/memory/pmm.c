#include <device/serial/serial.h>
#include <device/display/terminal.h>
#include <device/display/log.h>
#include <limine/limine.h>
#include <memory/pmm.h>
#include <stdlib.h>
#include <string.h>

static volatile struct limine_memmap_request request = {.id = LIMINE_MEMMAP_REQUEST, .revision = 0};

uint64_t pmm_get_blocks() {
  if (request.response->entry_count == 0) {
    return 0;
  }
  uint64_t c = 0;
  for (uint64_t i = 0; i < request.response->entry_count; i++) {
    struct limine_memmap_entry *current_entry = request.response->entries[i];
    c += (current_entry->length / BLOCK_SIZE);
  }
  return c;
}

void pmm_init(_block_t *blocks) {
  for (uint64_t i = 0; i < request.response->entry_count; i++) {
    struct limine_memmap_entry *current_entry = request.response->entries[i];
    switch (request.response->entries[i]->type) {
      case LIMINE_MEMMAP_USABLE:
        blocks[(current_entry->base + current_entry->length) / BLOCK_SIZE].block_type = FREE;
        break;

      default:
        blocks[(current_entry->base + current_entry->length) / BLOCK_SIZE].block_type = RESERVED;
        break;
    }
  }
}