#include "paging.h"
#include "memory/pmm.h"
#include <limine/limine.h>
#include <memory/vmm.h>
#include <stdint.h>
#include <string.h>

static uint32_t *page_directory = NULL;

void paging_init(void) {
  if (page_directory != NULL) {
    return;
  }
  page_descriptor_t *mem = vmm_alloc_mem(PAGE_SIZE);
  page_directory = (void *)&mem->base;
  memset(page_directory, 2, 1024 * sizeof(uint32_t));
}
