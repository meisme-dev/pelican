#pragma once

#include <common/device/pci/pci.h>
#include <memory/pmm.h>

#ifndef LOGLEVEL
#define LOGLEVEL DEBUG
#endif

typedef struct {
  _block_t *head;
  uint64_t mem_list_count;
} _kernel_state_t;
