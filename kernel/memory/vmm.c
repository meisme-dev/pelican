#include "vmm.h"
#include "pmm.h"
#include "sync/lock.h"

// TODO: Place allocated page descriptors in the current processe's descriptor
page_descriptor_t *vmm_alloc_mem(uint64_t size, virtual_memory_object_t *object) {
  static atomic_flag lock = ATOMIC_FLAG_INIT;
  acquire(&lock);

  uint64_t length = ((size + PAGE_SIZE - 1) / PAGE_SIZE) * PAGE_SIZE;

  virtual_memory_object_t *current_object = object;

  while (current_object->next != NULL) {
    current_object = (virtual_memory_object_t *)current_object->next;
    virtual_memory_object_t *prev_object = (virtual_memory_object_t *)current_object->prev;
    uintptr_t base = (prev_object == NULL ? 0 : prev_object->base);

    if (base + length < current_object->base) {
    }
  }

  /* Allocate an initial head page */
  page_descriptor_t *current_page = pmm_alloc_page();

  /* Return if the allocation fails */
  if (!current_page) {
    release(&lock);
    return NULL;
  }

  /* Loop over the size and determine the amount of pages to allocate */
  for (uint64_t i = 0; i < length / PAGE_SIZE; i++) {
    /* Get the last page descriptor in the list */
    while (current_page->next) {
      current_page = current_page->next;
    }

    /* Allocate a new page, breaking if it fails */
    page_descriptor_t *new_page = pmm_alloc_page();
    if (!new_page) {
      break;
    }

    /* Insert the new page */
    current_page->next = new_page;
    new_page->prev = current_page;
  }

  /* Jump back to the first page */
  /* TODO: Optimize */
  while (current_page->prev) {
    current_page = current_page->prev;
  }

  release(&lock);
  /* Return the pointer to the head page */
  return current_page;
}

void vmm_free_mem(page_descriptor_t *header) {
  /* Do not attempt to free a NULL page descriptor */
  if (header == NULL) {
    return;
  }
  static atomic_flag lock = ATOMIC_FLAG_INIT;
  acquire(&lock);

  page_descriptor_t *current_header = header;

  /* Loop over each page_descriptor, freeing it */
  while (current_header->next) {
    current_header = current_header->next;
    pmm_free_page(current_header->prev);
    current_header->prev = NULL;
  }
  release(&lock);
}
