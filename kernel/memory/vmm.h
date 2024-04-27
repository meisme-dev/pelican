#pragma once

#include "pmm.h"

page_descriptor_t *vmm_alloc_mem(uint64_t size);

void vmm_free_mem(page_descriptor_t *page);
