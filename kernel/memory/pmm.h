#ifndef PMM_H
#define PMM_H

#include <stdbool.h>
#include <stdint.h>

#include "../../libk/sys/list.h"

#define BLOCK_SIZE 4096

typedef struct {
    Node node;
    bool is_free;
    uint64_t index;
} Block;

void init_pmm();

#endif