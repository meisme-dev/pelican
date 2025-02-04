#pragma once

#include <arch/common/cpu/arch.h>
#include <memory/pmm.h>
#include <stddef.h>
#include <sync/lock.h>

typedef struct {
  size_t pid;
  task_context_t context;
  uint8_t priority : 7;
  uint8_t privilege : 1;
  size_t *root_page_table;
  page_descriptor_t *memory_regions;
  atomic_flag lock;
} task_t;
