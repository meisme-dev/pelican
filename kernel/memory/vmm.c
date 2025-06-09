#include "vmm.h"
#include "arch/amd64/memory/vmm.h"
#include "arch/common/memory/vmm.h"
#include "exception/panic.h"
#include "kernel.h"
#include "pmm.h"
#include "sync/lock.h"
#include <stdint.h>

void *vmm_alloc_mem(uint64_t size, virtual_memory_object_t *object, uintptr_t **pt_root, size_t flags) {
  static atomic_flag lock = ATOMIC_FLAG_INIT;
  acquire(&lock);

  /* Round the length up to the nearest page */
  uint64_t length = ROUND_UP(size, PAGE_SIZE);
  uintptr_t address = 0;

  virtual_memory_object_t *current_object = object;

  /* Move through the list of virtual memory objects until a suitable region is found */
  while (current_object != NULL && current_object->next != NULL) {
    current_object = (virtual_memory_object_t *)current_object->next;
    virtual_memory_object_t *prev_object = (virtual_memory_object_t *)current_object->prev;
    uintptr_t base = (prev_object == NULL ? 0 : prev_object->base);

    /* Attempt to find a free region between objects */
    if (base + length < current_object->base) {
      address = base;
      break;
    }
  }

  /* If no such region is found and the current object's base is also 0, we're probably the first object */
  /* Otherwise, if no such region is found, allocate a new object */
  if (address == 0 && current_object->base == 0) {
    address = vmm_get_dynamic_memory_base();
  } else if (address == 0) {
    uintptr_t base = 0;

    if (ROUND_UP((uintptr_t)current_object, PAGE_SIZE) - (uintptr_t)current_object > sizeof(virtual_memory_object_t) ||
        ROUND_UP((uintptr_t)current_object, PAGE_SIZE) == (uintptr_t)current_object) {
      base = (uintptr_t)current_object + sizeof(virtual_memory_object_t);
    } else {
      page_descriptor_t *page = pmm_alloc_page();

      if (page == NULL || page->base == 0) {
        panic("OUT OF MEMORY");
      }

      base = page->base + vmm_get_direct_map_base();
    }

    virtual_memory_object_t *new_object = (virtual_memory_object_t *)base;
    current_object->next = (void *)new_object;
    new_object->prev = (void *)current_object;

    if (object->base != vmm_get_dynamic_memory_base() && size <= PAGE_SIZE) {
      address = vmm_get_dynamic_memory_base();
    } else {
      address = current_object->base + current_object->length;
    }

    current_object = (virtual_memory_object_t *)new_object;
  }

  /* Set the address and size of the object */
  current_object->base = address;
  current_object->length = size;

  /* Allocate an initial head page */
  page_descriptor_t *first_page = pmm_alloc_page();
  page_descriptor_t *current_page = first_page;

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

    /* Map the memory */
    /* TODO: Handle the flags for different architectures */
    vmm_map(current_page->base, address + (i * PAGE_SIZE), flags, pt_root);
  }

  /* Jump back to the first page */
  current_page = first_page;

  release(&lock);

  /* Return the pointer to the first page */
  return (void *)address;
}

void vmm_free_mem(void *ptr, virtual_memory_object_t *object, uintptr_t **pt_root) {
  static atomic_flag lock = ATOMIC_FLAG_INIT;
  acquire(&lock);

  if (object == NULL) {
    release(&lock);
    return;
  }

  virtual_memory_object_t *current_object = object;

  /* Attempt to find pointer object */
  while (current_object->next != NULL) {
    if ((uintptr_t)ptr == current_object->base) {
      break;
    }

    current_object = (void *)current_object->next;
  }

  /* TODO: Throw error if pointer does not exist */
  if (current_object->base != (uintptr_t)ptr) {
    panic("Attempted to free non-existent pointer");
    release(&lock);
    return;
  }

  /* Loop over each page_descriptor, freeing it */
  for (size_t i = 0; i < current_object->length / PAGE_SIZE; i++) {
    uintptr_t phys = vmm_get_phys(current_object->base + (i * PAGE_SIZE), pt_root);
    vmm_unmap(current_object->base + (i * PAGE_SIZE), pt_root);
    pmm_free_page((void *)phys);
  }

  /* Drop the virtual memory object from the list  */
  virtual_memory_object_t *next_obj = (virtual_memory_object_t *)current_object->next;
  virtual_memory_object_t *prev_obj = (virtual_memory_object_t *)current_object->prev;

  if (prev_obj != NULL) {
    prev_obj->next = (void *)next_obj;
  }

  if (next_obj != NULL) {
    next_obj->prev = (void *)prev_obj;
  }

  if ((uintptr_t)current_object == ROUND_DOWN((uintptr_t)current_object, PAGE_SIZE)) {
    pmm_free_page(current_object);
  }

  release(&lock);
}
