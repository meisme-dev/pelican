#pragma once

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#define PAGE_SIZE 4096

typedef struct page_descriptor_t {
  struct page_descriptor_t *next;
} page_descriptor_t;

page_descriptor_t *pmm_init(void);

void *pmm_alloc_page(void);

uint64_t pmm_get_total_mem(void);

struct limine_memmap_response *pmm_get_memmap(void);

void pmm_free_page(void *addr);
