#pragma once

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

#include <sys/list.h>

#define BLOCK_SIZE 4096

enum block_type {
    FREE,
    USED,
};

typedef struct {
    bool used;
    uint64_t index;
} _block_t /* __attribute__((aligned(BLOCK_SIZE))) */;

void *pmm_init(uint64_t *count);
