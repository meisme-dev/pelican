#pragma once

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include <sys/list.h>

#define BLOCK_SIZE 4096

enum block_types {
  FREE,
  USED,
  RESERVED,
};

typedef struct {
  enum block_types block_type;
} _block_t;

uint64_t pmm_get_blocks();
void pmm_init(_block_t *blocks);