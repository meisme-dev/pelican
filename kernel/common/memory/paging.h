#pragma once
#include <memory/pmm.h>

void paging_init(void);
void paging_map(size_t src, size_t dst);
void paging_unmap(void *addr);
