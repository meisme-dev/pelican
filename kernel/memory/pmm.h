#pragma once

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#define BLOCK_SIZE 4096

enum block_types {
  FREE,
  USED
};

typedef struct page_header_t {
  enum block_types block_type;
  uint64_t base;
  struct page_header_t *next;
} page_header_t __attribute__((aligned(BLOCK_SIZE)));

page_header_t *pmm_init();
page_header_t *pmm_alloc_block();
void pmm_free_block(page_header_t *header);