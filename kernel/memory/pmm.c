#include "pmm.h"
#include <exception/panic.h>
#include <limine/limine.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <sync/lock.h>
#include <terminal/log.h>
#include <terminal/terminal.h>

static volatile struct limine_memmap_request request = {.id = LIMINE_MEMMAP_REQUEST, .revision = 0};
static page_descriptor_t *page_head = NULL;
static uint64_t total_mem = 0;

static void pmm_allocate_list(void) {
  for (uint64_t i = 0; i < request.response->entry_count; i++) {
    struct limine_memmap_entry *current_entry = request.response->entries[i];

    /* If the current entry is not usable, or not large enough, skip */
    if (current_entry->type != LIMINE_MEMMAP_USABLE || current_entry->length < (sizeof(page_descriptor_t) * 2) + PAGE_SIZE) {
      continue;
    }

    uint64_t descriptor_count = current_entry->length / PAGE_SIZE;
    page_descriptor_t *entry_page_head = (page_descriptor_t *)current_entry->base;

    /* Zero out the first 2 descriptors to prevent garbage data */
    memset(entry_page_head, 0, sizeof(page_descriptor_t));
    memset(&entry_page_head[1], 0, sizeof(page_descriptor_t));

    /* If the current page descriptor is not initialized, set it to the first descriptor of this entry */
    if (page_head == NULL) {
      page_head = entry_page_head;
      page_head->next = &entry_page_head[1];
      entry_page_head[1].prev = page_head;
    }

    page_descriptor_t *current_page_descriptor = page_head;

    /* Get the last entry, and verify that it is not out of range */
    while (current_page_descriptor->next != NULL && (uint64_t)current_page_descriptor->next < current_entry->base + current_entry->length) {
      current_page_descriptor = current_page_descriptor->next;
    }

    for (uint64_t j = 0; j < descriptor_count; j++) {
      page_descriptor_t *new_page_descriptor = &entry_page_head[j];
      memset(new_page_descriptor, 0, sizeof(page_descriptor_t));

      /* Take into account the page descriptor size when setting the base */
      new_page_descriptor->base = current_entry->base + (j * PAGE_SIZE) + (sizeof(page_descriptor_t) * descriptor_count);
      current_page_descriptor->base = current_entry->base + (j * PAGE_SIZE) + (sizeof(page_descriptor_t) * descriptor_count);

      /* Insert the new page descriptor */
      current_page_descriptor->next = new_page_descriptor;
      new_page_descriptor->prev = current_page_descriptor;
      current_page_descriptor = new_page_descriptor;

      total_mem += PAGE_SIZE;
    }
  }
}

page_descriptor_t *pmm_alloc_page(void) {
  static atomic_flag lock = ATOMIC_FLAG_INIT;
  acquire(&lock);
  /* Take the next usable page, instead of taking page_head */
  page_descriptor_t *allocated_page = page_head->next;

  if (allocated_page->next == NULL || allocated_page == NULL) {
    /* Return as there is likely not enough free memory */
    release(&lock);
    return NULL;
  }

  /* Drop the descriptor from the free list, so that nothing else tries to allocate it */
  page_head->next = allocated_page->next;
  page_head->next->prev = page_head;
  allocated_page->next = NULL;
  allocated_page->prev = NULL;
  release(&lock);
  return allocated_page;
}

void pmm_free_page(page_descriptor_t *descriptor) {
  static atomic_flag lock = ATOMIC_FLAG_INIT;
  acquire(&lock);

  /* Get the last entry in the free list */
  page_descriptor_t *current_page_descriptor = page_head;
  while (current_page_descriptor->next != NULL) {
    current_page_descriptor = current_page_descriptor->next;
  }

  /* Insert the descriptor */
  current_page_descriptor->next = descriptor;
  descriptor->prev = current_page_descriptor;
  descriptor->next = NULL;
  release(&lock);
}

uint64_t pmm_get_total_mem(void) {
  return total_mem;
}

page_descriptor_t *pmm_init(void) {
  pmm_allocate_list();
  page_descriptor_t *current_page = page_head;
  while (current_page->next) {
    current_page = current_page->next;
  }
  log_print(SUCCESS, "Initialized PMM");
  return page_head;
}
