#include "memory/pmm.h"
#define _ALLOC_SKIP_DEFINE
#include "liballoc.h"

#include <arch/common/memory/vmm.h>
#include <memory/vmm.h>
#include <sync/lock.h>

static atomic_flag lock = ATOMIC_FLAG_INIT;

int liballoc_lock() {
  acquire(&lock);
  return 0;
}

int liballoc_unlock() {
  release(&lock);
  return 0;
}

void *liballoc_alloc(int pages) {
  page_descriptor_t *page = vmm_alloc_mem(pages * PAGE_SIZE);
  return (void *)page->base;
}
