#ifndef PMM_H
#define PMM_H

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

#include <sys/list.h>

#define BLOCK_SIZE 4096

typedef struct {
    Node node;
    uint64_t index;
    bool isFree;
} Block __attribute__((aligned(BLOCK_SIZE)));

void init_pmm(void);

#endif
