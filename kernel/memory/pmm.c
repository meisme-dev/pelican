#include <device/serial/serial.h>
#include <limine/limine.h>
#include <memory/pmm.h>
#include <stdlib.h>
#include <string.h>

static volatile struct limine_memmap_request request = {.id = LIMINE_MEMMAP_REQUEST, .revision = 0};

Block *head = NULL;

void *init_pmm(uint64_t *count) {
  if (request.response->entry_count == 0) {
    return NULL;
  }

  uint64_t total_blocks = 0;
  uint64_t total_mem = 0;
  for (uint64_t i = 0; i < request.response->entry_count; i++) {
    total_mem += request.response->entries[i]->length;
  }
  total_blocks = total_mem / BLOCK_SIZE;

  uint64_t c = 0;
  uint64_t reserved_start = 0;
  uint64_t reserved_end = 0;
  bool reserved = false;

  for (uint64_t i = 0; i < request.response->entry_count; i++) {
    switch (request.response->entries[i]->type) {
      case LIMINE_MEMMAP_USABLE:
        if (request.response->entries[i]->length > sizeof(Block) * total_blocks && reserved == false) {
          head = (Block *)request.response->entries[i]->base;
          reserved = true;
          reserved_start = request.response->entries[i]->base / BLOCK_SIZE;
          for (uint64_t offset = 0; offset < request.response->entries[i]->length / BLOCK_SIZE; offset++, c++) {
            uint64_t index = (request.response->entries[i]->base / BLOCK_SIZE) + offset;
            head[c].used = USED;
            head[c].index = index;
          }
          reserved_end = head[c].index;
        }
        break;
      default:
        break;
    }
  }

  for (uint64_t i = 0; i < request.response->entry_count; i++) {
    switch (request.response->entries[i]->type) {
      case LIMINE_MEMMAP_USABLE:
        for (uint64_t offset = 0; offset < request.response->entries[i]->length / BLOCK_SIZE; offset++, c++) {
          uint64_t index = (request.response->entries[i]->base / BLOCK_SIZE) + offset;
          if(index > reserved_start && index < reserved_end) {
            break;
          }
          head[c].used = FREE;
          head[c].index = index;
        }
        break;
      default:
        for (uint64_t offset = 0; offset < request.response->entries[i]->length / BLOCK_SIZE; offset++, c++) {
          uint64_t index = (request.response->entries[i]->base / BLOCK_SIZE) + offset;
          head[c].used = USED;
          head[c].index = index;
        }
        break;
    }
  }

  *count = c;
  return head;
}