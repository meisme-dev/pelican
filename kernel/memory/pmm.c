#include <stddef.h>

#include "pmm.h"
#include "../../vendor/limine/limine.h"
#include "../device/display/terminal.h"
#include "../../libk/log.h"

static volatile struct limine_memmap_request request = {
    .id = LIMINE_MEMMAP_REQUEST,
    .revision = 0
};

static Block *head = 0x0;
static uint64_t *start_addr = 0x0;

void init_pmm() {
    if(request.response->entries == NULL || request.response->entry_count == 0) {
        return;
    }
    trace();
    uint64_t total_mem = 0;
    for(uint64_t i = 0; i < request.response->entry_count; i++) {
        total_mem += request.response->entries[i]->length;
    }
    for(uint64_t i = 0; i < request.response->entry_count; i++) {
        switch(request.response->entries[i]->type) {
        case LIMINE_MEMMAP_USABLE:
            if(start_addr == NULL && request.response->entries[i]->length >= ((total_mem / BLOCK_SIZE) * sizeof(Block))) {
                start_addr = (uint64_t *)(request.response->entries[i]->base + ((total_mem / BLOCK_SIZE) * sizeof(Block)));
                printf("    Reserving %b for free list\n", (total_mem / BLOCK_SIZE) * sizeof(Block));
            }
            printf("    Usable memory: base: %b, length: %b\n", request.response->entries[i]->base, request.response->entries[i]->length);
            break;
        
        default:
            printf("    Reserved memory: base: %b, length: %b\n", request.response->entries[i]->base, request.response->entries[i]->length);
            break;
        }
    }
    printf("Total memory: %b\n", (total_mem));
    printf("Start offset: %b\n", start_addr);
    head = (Block *)start_addr;
}

Block *find_free_block() {
    return (Block *)head->node.next;
}

void kfree(void *addr) {
    Block *block = head;
    while(block->node.next != NULL) {
        block = (Block *)block->node.next;
        if((void *)(start_addr + (block->index * BLOCK_SIZE)) == addr) {
            Block *prev = (Block *)block->node.prev;
            prev->node.next = block->node.next;
            if (block->node.next != NULL) {
                Block *next = (Block *)block->node.next;
                next->node.prev = prev;
            }
        }
    }
}