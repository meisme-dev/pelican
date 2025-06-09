#pragma once

#include "memory/vmm.h"
#include <arch/common/cpu/arch.h>
#include <memory/pmm.h>
#include <stddef.h>
#include <stdint.h>
#include <sync/lock.h>

typedef enum {
  WAITING,
  RUNNING,
  SLEEPING,
  DEAD
} task_status_t;

typedef struct {
  size_t pid;
  task_context_t context;
  uint8_t priority : 7;
  uint8_t privilege : 1;
  uintptr_t *root_page_table;
  virtual_memory_object_t *memory_regions;
  atomic_flag lock;
  task_status_t status;
  struct task *next;
} task_t;

task_t *task_add(uint8_t priority, uint8_t privilege);
void task_init();
