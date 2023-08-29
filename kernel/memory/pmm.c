#include <common/exception/panic.h>
#include <common/io/serial/serial.h>
#include <limine/limine.h>
#include <memory/pmm.h>
#include <stdlib.h>
#include <string.h>
#include <terminal/log.h>
#include <terminal/terminal.h>

static volatile struct limine_memmap_request request = {.id = LIMINE_MEMMAP_REQUEST, .revision = 0};

static page_header_t *page_head = 0x0;

static void pmm_allocate_list() {
  for (uint64_t i = 0; i < request.response->entry_count; i++) {
    struct limine_memmap_entry *current_entry = request.response->entries[i];
    if (current_entry->type != LIMINE_MEMMAP_USABLE || current_entry->length < sizeof(page_header_t) + BLOCK_SIZE) {
      continue;
    }

    if (page_head == NULL) {
      page_head = (page_header_t *)(current_entry->base);
      memset(page_head, 0, sizeof(page_header_t));
    }

    page_header_t *entry_page_head = (page_header_t *)current_entry->base;

    if (page_head->next == NULL) {
      page_head->next = &entry_page_head[1];
    }

    page_header_t *current_page_header = page_head;

    while (current_page_header->next) {
      current_page_header = (page_header_t *)current_page_header->next;
    }
    for (uint64_t j = 0; j < current_entry->length / BLOCK_SIZE; j++) {
      page_header_t *new_page_header = &entry_page_head[j];
      new_page_header->base = current_entry->base + (j * BLOCK_SIZE);

      current_page_header->base = current_entry->base + (j * BLOCK_SIZE);
      current_page_header->next = new_page_header;
      current_page_header->block_type = FREE;
    }
  }
}

page_header_t *pmm_alloc_block() {
  page_header_t *current_page = page_head;
  while (current_page->next) {
    current_page = current_page->next;
    if (current_page->block_type == USED) {
      continue;
    }
    current_page->block_type = USED;
    return current_page;
  }
  return NULL;
}

void pmm_free_block(page_header_t *header) {
  if (header == NULL) {
    return;
  }
  header->block_type = FREE;
}

page_header_t *pmm_init() {
  pmm_allocate_list();
  log(SUCCESS, "Initialized PMM");
  return page_head;
}
