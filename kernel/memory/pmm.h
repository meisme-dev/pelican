#pragma once

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#define BLOCK_SIZE 4096

enum block_types {
  FREE,
  USED
};

typedef struct {
  bool block_type;
} _block_t;

_block_t *pmm_get_blocks(uint64_t *c);
void pmm_init(_block_t *blocks, uint64_t count);