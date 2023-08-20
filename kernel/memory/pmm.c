#include <common/io/serial/serial.h>
#include <limine/limine.h>
#include <memory/pmm.h>
#include <stdlib.h>
#include <string.h>
#include <video/log.h>
#include <video/terminal.h>

static volatile struct limine_memmap_request request = {.id = LIMINE_MEMMAP_REQUEST, .revision = 0};

_block_t *pmm_get_blocks(uint64_t *c) {
  if (request.response->entry_count == 0) {
    return NULL;
  }

  for (uint64_t i = 0; i < request.response->entry_count; i++) {
    struct limine_memmap_entry *current_entry = request.response->entries[i];
    switch (request.response->entries[i]->type) {
      case LIMINE_MEMMAP_USABLE:
        log(DEBUG, "Usable: base: %u length: %u", current_entry->base / BLOCK_SIZE, current_entry->length / BLOCK_SIZE);
        break;
      default:
        log(DEBUG, "Reserved: base: %u length: %u", current_entry->base / BLOCK_SIZE, current_entry->length / BLOCK_SIZE);
        break;
    }
    *c += (current_entry->length) / BLOCK_SIZE;
  }

  for (uint64_t i = 0; i < request.response->entry_count; i++) {
    struct limine_memmap_entry *current_entry = request.response->entries[i];
    switch (request.response->entries[i]->type) {
      case LIMINE_MEMMAP_USABLE:
        if (current_entry->length > *c * sizeof(_block_t)) {
          return (_block_t *)current_entry->base;
        }
        break;
    }
  }

  return NULL;
}

void pmm_init(_block_t *blocks, uint64_t count) {
  memset(blocks, 1, count);
  for (uint64_t i = 0; i < request.response->entry_count; i++) {
    struct limine_memmap_entry *current_entry = request.response->entries[i];
    switch (request.response->entries[i]->type) {
      case LIMINE_MEMMAP_USABLE:
        for (int32_t j = (current_entry->base + current_entry->length) / BLOCK_SIZE; j >= 0 && j < (int32_t)count; j--) {
          blocks[j].block_type = FREE;
        }
        break;
    }
  }
  log(SUCCESS, "Initialized PMM");
}