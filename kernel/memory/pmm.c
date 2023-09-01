#include <common/exception/panic.h>
#include <common/io/serial/serial.h>
#include <limine/limine.h>
#include <memory/pmm.h>
#include <stdlib.h>
#include <string.h>
#include <sync/lock.h>
#include <terminal/log.h>
#include <terminal/terminal.h>

static volatile struct limine_memmap_request request = {.id = LIMINE_MEMMAP_REQUEST, .revision = 0};

static page_header_t *page_head = NULL;

static void pmm_allocate_list() {
  for (uint64_t i = 0; i < request.response->entry_count; i++) {
    struct limine_memmap_entry *current_entry = request.response->entries[i];

    if (current_entry->type != LIMINE_MEMMAP_USABLE || current_entry->length < (sizeof(page_header_t) * 2) + BLOCK_SIZE) {
      continue;
    }

    page_header_t *entry_page_head = (page_header_t *)current_entry->base;
    memset(entry_page_head, 0, sizeof(page_header_t));
    memset(&entry_page_head[1], 0, sizeof(page_header_t));

    if (page_head == NULL) {
      page_head = entry_page_head;
      page_head->next = &entry_page_head[1];
    }

    page_header_t *current_page_header = page_head;

    while (current_page_header->next != NULL && (uint64_t)current_page_header->next < current_entry->base + current_entry->length) {
      current_page_header = current_page_header->next;
    }

    for (uint64_t j = 0; j < current_entry->length / BLOCK_SIZE; j++) {
      page_header_t *new_page_header = &entry_page_head[j];
      memset(new_page_header, 0, sizeof(page_header_t));
      new_page_header->base = current_entry->base + (j * BLOCK_SIZE);
      current_page_header->base = current_entry->base + (j * BLOCK_SIZE);
      current_page_header->next = new_page_header;
      current_page_header->block_type = current_page_header->base < current_entry->base + (sizeof(page_header_t) * 2) ? USED : FREE;
    }
  }
}

page_header_t *pmm_alloc_block() {
  static atomic_flag lock = ATOMIC_FLAG_INIT;
  acquire(&lock);
  page_header_t *current_page = page_head;
  while (current_page->next) {
    current_page = current_page->next;
    if (current_page->block_type == USED) {
      continue;
    }
    current_page->block_type = USED;
    release(&lock);
    return current_page;
  }
  release(&lock);
  return NULL;
}

void pmm_free_block(page_header_t *header) {
  if (header == NULL) {
    return;
  }
  static atomic_flag lock = ATOMIC_FLAG_INIT;
  acquire(&lock);
  header->block_type = FREE;
  release(&lock);
}

page_header_t *pmm_init() {
  pmm_allocate_list();
  log(SUCCESS, "Initialized PMM");
  return page_head;
}
