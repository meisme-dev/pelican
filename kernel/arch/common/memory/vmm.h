#pragma once

#include <stddef.h>
#include <stdint.h>

uintptr_t *vmm_init(void);
void vmm_map(size_t src, size_t dst, size_t flags, uintptr_t **page_map_level_4);
void vmm_unmap(void *addr);
