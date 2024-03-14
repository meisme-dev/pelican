#include "paging.h"
#include "common/exception/panic.h"
#include "memory/pmm.h"
#include "terminal/log.h"
#include <limine/limine.h>
#include <memory/vmm.h>
#include <stdint.h>
#include <string.h>

static uint64_t *page_directory = NULL;
static uint64_t *page_pointer_table = NULL;
static uint64_t *first_page_table = NULL;

#define ROUND_UP(x, y) ((((x) + (y)-1) / (y)) * (y))
#define ROUND_DOWN(x, y) ((x / y) * y)
#define ROUND(x, y) ROUND_UP(x, y) - x < x - ROUND_DOWN(x, y) ? ROUND_UP(x, y) : ROUND_DOWN(x, y)

void paging_init(void) {
  if (page_directory != NULL) {
    return;
  }
  page_descriptor_t *mem = vmm_alloc_mem(1024 * sizeof(uint32_t) * 2);

  if ((uint32_t)mem->base != mem->base) {
    panic("Could not find an address that would fit in CR3");
  }

  page_directory = (void *)&mem->base;
  first_page_table = (void *)(&mem->base + (512 * sizeof(uint64_t)));

  memset(page_directory, 2, 512 * sizeof(uint64_t));

  for (uint16_t i = 0; i < 512; i++) {
    first_page_table[i] = (i * 0x1000) | 3;
  }

  page_directory[0] = (uint64_t)first_page_table | 3;

  log_print(SUCCESS, "Initialiazed paging");
}

void paging_map(uint64_t src, uint64_t dst) {
  uint64_t pdpt_index = ROUND(src, 4096 * 262144) / (4096 * 262144);
  uint64_t pd_index = ROUND(src, 4096 * 512);
}
