#include "task.h"
#include "arch/amd64/memory/vmm.h"
#include "exception/panic.h"
#include "kernel.h"
#include "memory/pmm.h"

#include <arch/common/memory/vmm.h>
#include <stdatomic.h>
#include <stdint.h>

static size_t pid_counter = 0;

static task_t *task_head = NULL;
static task_t *task_tail = NULL;

void task_init() {
  kernel_task = task_add(0xf, 1);
}

task_t *task_add(uint8_t priority, uint8_t privilege) {
  page_descriptor_t *object_head_page = pmm_alloc_page();

  if (object_head_page == NULL || object_head_page->base == 0) {
    panic("OUT OF MEMORY");
  }

  uintptr_t base = 0;

  if (task_head == NULL) {
    page_descriptor_t *page = pmm_alloc_page();
    if (page == NULL || page->base == 0) {
      panic("OUT OF MEMORY");
    }

    task_head = (void *)(page->base + vmm_get_direct_map_base());
    task_tail = task_head;
  }

  if (ROUND_UP((uintptr_t)task_head, PAGE_SIZE) - (uintptr_t)task_head > sizeof(task_t) ||
      ROUND_UP((uintptr_t)task_head, PAGE_SIZE) == (uintptr_t)task_head) {
    base = (uintptr_t)task_head + sizeof(virtual_memory_object_t);
  } else {
    page_descriptor_t *page = pmm_alloc_page();

    if (page == NULL || page->base == 0) {
      panic("OUT OF MEMORY");
    }

    base = page->base + vmm_get_direct_map_base();
  }

  task_t *new_task = (void *)base;

  task_tail->next = (void *)new_task;
  task_tail = (void *)task_tail->next;

  new_task->pid = pid_counter;
  new_task->memory_regions = (void *)(object_head_page->base + vmm_get_direct_map_base());
  new_task->lock = (atomic_flag)ATOMIC_FLAG_INIT;
  new_task->root_page_table = vmm_init();
  new_task->priority = priority;
  new_task->privilege = privilege;

  pid_counter++;

  return new_task;
}
