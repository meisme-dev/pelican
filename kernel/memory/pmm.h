#pragma once

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#define PAGE_SIZE 4096

enum page_types {
  FREE,
  USED
};

typedef struct page_header_t {
  enum page_types page_type;
  uint64_t base;
  struct page_header_t *next;
} page_header_t __attribute__((aligned(PAGE_SIZE)));

page_header_t *pmm_init();
page_header_t *pmm_alloc_page();
void pmm_free_page(page_header_t *header);