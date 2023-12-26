#pragma once
#include <memory/pmm.h>

void paging_init();
void paging_map_page(size_t src, size_t dst);
void paging_unmap_page(void *addr);

