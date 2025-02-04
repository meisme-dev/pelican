#pragma once

#include <arch/common/memory/vmm.h>
#include <stdint.h>

void vmm_load(uintptr_t address);
uint64_t vmm_get_direct_map_base();
