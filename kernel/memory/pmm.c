#include "pmm.h"
#include <exception/panic.h>
#include <kernel.h>
#include <limine/limine.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <sync/lock.h>
#include <terminal/log.h>
#include <terminal/terminal.h>

static volatile struct limine_memmap_request request = {.id = LIMINE_MEMMAP_REQUEST, .revision = 6};
static page_descriptor_t *page_head = NULL;
static uint64_t total_mem = 0;

uint64_t pmm_get_total_mem() {
  if (total_mem != 0) {
    return total_mem;
  }

  for (size_t i = 0; i < request.response->entry_count; i++) {
    struct limine_memmap_entry *current_entry = request.response->entries[i];

    if (current_entry->type != LIMINE_MEMMAP_BAD_MEMORY &&
        current_entry->type != LIMINE_MEMMAP_RESERVED &&
        current_entry->type != LIMINE_MEMMAP_FRAMEBUFFER) {
      total_mem += current_entry->length;
    }
  }

  return total_mem;
}

static void pmm_allocate_list(void) {
  page_descriptor_t *current_page = NULL;

  for (size_t i = 0; i < request.response->entry_count; i++) {
    struct limine_memmap_entry *current_entry = request.response->entries[i];

    log_print(SYSTEM, "(Memory map) Type: %u, Base, 0x%x, Size: 0x%x", current_entry->type, current_entry->base, current_entry->length);

    if (current_entry->type != LIMINE_MEMMAP_USABLE || current_entry->length < PAGE_SIZE) {
      continue;
    }

    if (page_head == NULL) {
      page_head = (page_descriptor_t *)current_entry->base;
      current_page = page_head;
    }

    for (uintptr_t i = current_entry->base; i < current_entry->base + current_entry->length; i += PAGE_SIZE) {
      current_page->next = (page_descriptor_t *)i;
      current_page = current_page->next;
    }
  }
}

void *pmm_alloc_page(void) {
  static atomic_flag lock = ATOMIC_FLAG_INIT;
  acquire(&lock);

  if (page_head == NULL || page_head->next == NULL) {
    release(&lock);
    return NULL;
  }

  page_descriptor_t *allocated_page = (page_descriptor_t *)page_head;
  page_head = page_head->next;

  memset((void *)(allocated_page), 0, PAGE_SIZE);

  release(&lock);
  return (void *)allocated_page;
}

void pmm_free_page(void *addr) {
  static atomic_flag lock = ATOMIC_FLAG_INIT;
  acquire(&lock);

  ((page_descriptor_t *)addr)->next = page_head;
  page_head = ((page_descriptor_t *)addr);

  release(&lock);
}

struct limine_memmap_response *pmm_get_memmap(void) {
  return request.response;
}

page_descriptor_t *pmm_init(void) {
  pmm_allocate_list();
  log_print(OK, "Initialized physical memory manager");
  log_print(INFO, "Detected memory: %u bytes", pmm_get_total_mem());
  return page_head;
}
