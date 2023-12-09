#pragma once

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#define PAGE_SIZE 4096

// enum page_types {
//   FREE,
//   USED
// };

typedef struct page_descriptor_t {
  // enum page_types page_type;
  uint64_t base;
  struct page_descriptor_t *next;
  struct page_descriptor_t *prev;
} page_descriptor_t __attribute__((aligned(PAGE_SIZE)));

/***************************************************************************
 * Initializes the physical memory manager
 * @return The free list head
 ***************************************************************************/
page_descriptor_t *pmm_init(void);

/***************************************************************************
 * Allocates a page by removing its descriptor from the free list
 * @return The page descriptor of the allocated page, may be NULL if no
 *         suitable descriptor is found
 ***************************************************************************/
page_descriptor_t *pmm_alloc_page(void);

/***************************************************************************
 * Frees a page by inserting it in the free list
 * @param[in] descriptor The descriptor to free
 ***************************************************************************/
void pmm_free_page(page_descriptor_t *descriptor);
