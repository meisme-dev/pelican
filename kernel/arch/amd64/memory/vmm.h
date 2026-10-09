#pragma once

#include <arch/common/memory/vmm.h>
#include <stddef.h>
#include <stdint.h>

void vmm_load(uintptr_t address);
uintptr_t vmm_get_direct_map_base();

#define VMM_FLAG_PRESENT 0b1
#define VMM_FLAG_READ_WRITE 0b10
#define VMM_FLAG_USER_SUPERVISOR 0b100
#define VMM_FLAG_WRITE_THROUGH 0b1000
#define VMM_FLAG_CACHE_DISABLE 0b10000
#define VMM_FLAG_ACCESSED 0b100000
