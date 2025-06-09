#include "pmm.h"
#include "arch/amd64/memory/vmm.h"
#include <exception/panic.h>
#include <kernel.h>
#include <limine/limine.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <sync/lock.h>
#include <terminal/log.h>
#include <terminal/terminal.h>

static volatile struct limine_memmap_request request = {.id = LIMINE_MEMMAP_REQUEST, .revision = 0};
static page_descriptor_t *page_head = NULL;
static page_descriptor_t *page_tail = NULL;
static uint64_t total_mem = 0;

static void pmm_allocate_list(void) {
  for (size_t i = 0; i < request.response->entry_count; i++) {
    struct limine_memmap_entry *current_entry = request.response->entries[i];

    log_print(DEBUG, "(Memory map) Type: %u, Base, 0x%x, Size: 0x%x", current_entry->type, current_entry->base, current_entry->length);

    if (current_entry->type != LIMINE_MEMMAP_BAD_MEMORY &&
        current_entry->type != LIMINE_MEMMAP_RESERVED &&
        current_entry->type != LIMINE_MEMMAP_FRAMEBUFFER) {
      total_mem += current_entry->length;
    }

    /* If the current entry is not usable, or not large enough, skip */
    if (current_entry->type != LIMINE_MEMMAP_USABLE || current_entry->length <= (current_entry->length / PAGE_SIZE) * sizeof(page_descriptor_t) + PAGE_SIZE) {
      continue;
    }

    uint64_t descriptor_count = current_entry->length / PAGE_SIZE;
    page_descriptor_t *entry_page_head = (page_descriptor_t *)(current_entry->base + vmm_get_direct_map_base());

    /* Zero out the first 2 descriptors to prevent garbage data */
    memset(entry_page_head, 0, sizeof(page_descriptor_t));
    memset(&entry_page_head[1], 0, sizeof(page_descriptor_t));

    /* If the current page descriptor is not initialized, set it to the first descriptor of this entry */
    if (page_head == NULL) {
      page_head = entry_page_head;
      page_head->next = (void *)((uintptr_t)(&entry_page_head[1]) + vmm_get_direct_map_base());
      entry_page_head[1].prev = page_head;
    }

    if (page_tail == NULL) {
      page_tail = page_head;
    }

    /* Get the last entry */
    page_descriptor_t *current_page_descriptor = page_tail;

    for (size_t k = 0; k < descriptor_count; k++) {
      page_descriptor_t *new_page_descriptor = &entry_page_head[k];
      memset(new_page_descriptor, 0, sizeof(page_descriptor_t));
      /* Take into account the page descriptor size when setting the base */
      new_page_descriptor->base = current_entry->base + (k * PAGE_SIZE) + ROUND_UP((sizeof(page_descriptor_t) * descriptor_count), PAGE_SIZE);

      /* Insert the new page descriptor */
      current_page_descriptor->next = new_page_descriptor;
      new_page_descriptor->prev = current_page_descriptor;
      current_page_descriptor = new_page_descriptor;
      page_tail = new_page_descriptor;
    }
  }
}

page_descriptor_t *pmm_alloc_page(void) {
  static atomic_flag lock = ATOMIC_FLAG_INIT;
  acquire(&lock);

  if (page_head == NULL || page_head->next == NULL) {
    release(&lock);
    return NULL;
  }

  /* Take the next usable page, instead of taking page_head */
  page_descriptor_t *allocated_page = (void *)page_head;

  memset((void *)(allocated_page->base + vmm_get_direct_map_base()), 0, PAGE_SIZE);

  /* Drop the descriptor from the free list, so that nothing else tries to allocate it */
  page_head = page_head->next;
  page_head->prev = NULL;

  allocated_page->next = NULL;
  allocated_page->prev = NULL;

  release(&lock);
  return allocated_page;
}

void pmm_free_page(void *addr) {
  static atomic_flag lock = ATOMIC_FLAG_INIT;
  acquire(&lock);

  /* Get the last entry in the free list */
  page_descriptor_t *current_page_descriptor = page_head;

  /* Create a new descriptor with the base */
  page_descriptor_t *descriptor = (void *)((uintptr_t)page_head - sizeof(page_descriptor_t));
  descriptor->base = (uintptr_t)addr;

  /* Insert the descriptor */
  current_page_descriptor->prev = descriptor;
  descriptor->next = current_page_descriptor;
  descriptor->prev = NULL;
  page_head = descriptor;

  release(&lock);
}

uint64_t pmm_get_total_mem(void) {
  return total_mem;
}

struct limine_memmap_response *pmm_get_memmap(void) {
  return request.response;
}

page_descriptor_t *pmm_init(void) {
  pmm_allocate_list();
  log_print(SUCCESS, "Initialized Physical Memory Manager");
  return page_head;
}
