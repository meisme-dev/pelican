#ifndef PMM_H
#define PMM_H

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
} Block /* __attribute__((aligned(BLOCK_SIZE))) */;

void *init_pmm(uint64_t *count);

#endif
