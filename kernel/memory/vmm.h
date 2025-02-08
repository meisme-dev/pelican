#pragma once

#include "pmm.h"

typedef struct {
  size_t base;
  size_t flags;
  size_t length;
  struct virtual_memory_object_t *next;
  struct virtual_memory_object_t *prev;
} virtual_memory_object_t;

page_descriptor_t *vmm_alloc_mem(uint64_t size, virtual_memory_object_t *object);

void vmm_free_mem(page_descriptor_t *page);
