#include <device/display/terminal.h>
#include <device/serial/serial.h>
#include <limine/limine.h>
#include <memory/pmm.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static volatile struct limine_memmap_request request = {
    .id = LIMINE_MEMMAP_REQUEST, .revision = 0};

static Block *head = 0x0;
static uint64_t *start_addr = 0x0;
static uint64_t index = 1;

uint64_t alloc_block(void);
bool free_block(uint64_t i);

#define SET_BLOCK_TYPES(x)                                                     \
  for (uint64_t j = 0; j < request.response->entries[i]->length;               \
       j += BLOCK_SIZE) {                                                      \
    index++;                                                                   \
    head[index - BLOCK_SIZE].node.next = &head[index];                         \
    head[index].node.prev = &head[index - BLOCK_SIZE];                         \
    head[index].type = x;                                                      \
  }

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
        SET_BLOCK_TYPES(RESERVED);
        break;
      }
      if (request.response->entries[i]->length < BLOCK_SIZE) {
        break;
      }
      SET_BLOCK_TYPES(FREE);
      break;

    default:
      SET_BLOCK_TYPES(RESERVED);
      break;
    }
  }

  head[index].node.next = NULL;
  printf("There are %d blocks\n", index);

  for (uint64_t i = 0; i < index; i++) {
    head[i].type == FREE ? serial_send(COM1, '#') : serial_send(COM1, '.');
  }

  uint64_t block = alloc_block();
  if (free_block(block)) {
    puts("Freed block");
  };
}

uint64_t alloc_block(void) {
  for (uint64_t i = 0; i < index; i++) {
    if (head[i].type == FREE) {
      head[i].type = USED;
      return i;
    }
  }
  return 0;
}

bool free_block(uint64_t i) {
  if (i > index || head[i].type == RESERVED) {
    return false;
  }
  head[i].type = FREE;
  return true;
}