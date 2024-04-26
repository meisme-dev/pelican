#pragma once

#include "pmm.h"

page_descriptor_t *vmm_alloc_mem(uint64_t size);

uintptr_t *vmm_init(void);
void vmm_map(size_t src, size_t dst, size_t flags, uintptr_t **page_map_level_4);
void vmm_unmap(void *addr);
void vmm_free_mem(page_descriptor_t *page);
