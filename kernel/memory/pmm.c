#include "device/serial/serial.h"
#include "stdlib.h"
#include <stdbool.h>
#include <stddef.h>

#include <device/display/terminal.h>
#include <limine/limine.h>
#include <memory/pmm.h>
#include <stdint.h>
#include <string.h>

static volatile struct limine_memmap_request request = {
    .id = LIMINE_MEMMAP_REQUEST, .revision = 0};

static Block *head = 0x0;
static uint64_t *start_addr = 0x0;
static uint64_t index = 1;

uint64_t alloc_block(void);

void init_pmm(void) {
  if (request.response->entries == NULL || request.response->entry_count == 0) {
    return;
  }

  uint64_t total_mem = 0;
  for (uint64_t i = 0; i < request.response->entry_count; i++) {
    total_mem += request.response->entries[i]->length;
  }

  for (uint64_t i = 0; i < request.response->entry_count; i++) {
    switch (request.response->entries[i]->type) {
    case LIMINE_MEMMAP_USABLE:
      if (start_addr == NULL &&
          request.response->entries[i]->length >=
              ((total_mem / BLOCK_SIZE) * sizeof(Block))) {
        start_addr = (uint64_t *)(request.response->entries[i]->base +
                                  ((total_mem / BLOCK_SIZE) * sizeof(Block)));
      }
      break;

    default:
      break;
    }
  }
  head = (Block *)start_addr;
  head->index = 0;
  bool free_list_allocd = false;
  for (uint64_t i = 0; i < request.response->entry_count; i++) {
    head[index].index = index;
    head[index].node.prev = (void *)&head[index - 1];
    head[index].node.next = NULL;
    switch (request.response->entries[i]->type) {
    case LIMINE_MEMMAP_USABLE:
      if (request.response->entries[i + 1]->base > (uint64_t)start_addr &&
          !free_list_allocd) {
        free_list_allocd = true;
        for (uint64_t j = 0; j < request.response->entries[i]->length;
             j += BLOCK_SIZE) {
          index++;
          head[index - BLOCK_SIZE].node.next = &head[index];
          head[index].node.prev = &head[index - BLOCK_SIZE];
          head[index].isFree = false;
        }
        break;
      }
      if (request.response->entries[i]->length < BLOCK_SIZE) {
        break;
      }
      for (uint64_t j = 0; j < request.response->entries[i]->length;
           j += BLOCK_SIZE) {
        index++;
        head[index - BLOCK_SIZE].node.next = &head[index];
        head[index].node.prev = &head[index - BLOCK_SIZE];
        head[index].isFree = true;
      }
      break;

    default:
      for (uint64_t j = 0; j < request.response->entries[i]->length;
           j += BLOCK_SIZE) {
        index++;
        head[index - BLOCK_SIZE].node.next = &head[index];
        head[index].node.prev = &head[index - BLOCK_SIZE];
        head[index].isFree = false;
      }
      break;
    }
  }
  head[index].node.next = NULL;
  printf("There are %d blocks\n", index);

  for (uint64_t i = 0; i < index; i++) {
    char str[3] = "";
    head[i].isFree ? strcpy(str, "#") : strcpy(str, ".");
    for (uint8_t i = 0; i < 3; i++) {
      put_serial(COM1, str[i]);
    }
  }
  alloc_block();
}

uint64_t alloc_block(void) {
  for(uint64_t i = 0; i < index; i++) {
    if(head[i].isFree) {
      head[i].isFree = false;
      return i;
    }
  }
  return 0;
}

bool free_block(uint64_t i) {
  if(i > index || head[i].isFree) {
    return false;
  }
  head[i].isFree = true;
  return true;
}
