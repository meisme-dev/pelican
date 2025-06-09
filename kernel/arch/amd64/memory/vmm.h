#pragma once

#include <arch/common/memory/vmm.h>
#include <stddef.h>
#include <stdint.h>

void vmm_load(uintptr_t address);
uintptr_t vmm_get_direct_map_base();
