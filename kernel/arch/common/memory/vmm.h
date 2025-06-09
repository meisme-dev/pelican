#pragma once

#include <stddef.h>
#include <stdint.h>

uintptr_t *vmm_init(void);
void vmm_map(size_t src, size_t dst, size_t flags, uintptr_t **page_map_level_4);
void vmm_unmap(size_t addr, uintptr_t **page_map_level_4);
size_t vmm_get_dynamic_memory_base(void);
uintptr_t vmm_get_phys(size_t addr, uintptr_t **page_map_level_4);
void *vmm_get_free(uintptr_t **page_map_level_4);
