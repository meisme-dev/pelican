#pragma once

#include "pmm.h"
#include "sync/lock.h"

page_header_t *vmm_alloc_mem(uint64_t size);
