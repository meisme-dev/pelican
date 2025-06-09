#pragma once

#include "pmm.h"

typedef struct {
  size_t base;
  size_t flags;
  size_t length;
  struct virtual_memory_object_t *next;
  struct virtual_memory_object_t *prev;
} virtual_memory_object_t;

void *vmm_alloc_mem(uint64_t size, virtual_memory_object_t *object, uintptr_t **pt_root, size_t flags);

void vmm_free_mem(void *ptr, virtual_memory_object_t *object, uintptr_t **pt_root);
