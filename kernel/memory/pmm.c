#include <stddef.h>

#include "pmm.h"
#include "../../vendor/limine/limine.h"
#include "../../libk/string.h"
#include "../device/display/terminal.h"

static volatile struct limine_memmap_request request = {
    .id = LIMINE_MEMMAP_REQUEST,
    .revision = 0
};

static Block head = {0};
static uint64_t start_addr = 0;

void init_pmm() {
    if(request.response->entries == NULL || request.response->entry_count == 0) {
        return;
    }
    printf("pmm.c\n");
    for(uint64_t i = 0; i < request.response->entry_count; i++) {
        switch(request.response->entries[i]->type) {
        case LIMINE_MEMMAP_USABLE:
            printf("Usable memory: base: %u, length: %u\n", request.response->entries[i]->base, request.response->entries[i]->length);
            break;
        
        default:
            printf("Reserved memory: base: %d, length: %d\n", request.response->entries[i]->base, request.response->entries[i]->length);
            break;
        }
    }
}

Block find_free_block() {
    Block block = head;
    while(!block.is_free && block.node.next != NULL) {
        block = *(Block *)block.node.next;
    }
    if(!block.is_free) {
        return head;
    }
    return block;
}

void *kmalloc(size_t x) {
    Block block[x / BLOCK_SIZE];
    uint64_t i = 0;
    uint64_t size = BLOCK_SIZE;
    while(size < x) {
        size += BLOCK_SIZE;
    }
    while(size > 0) {
        block[i] = find_free_block();
        size -= BLOCK_SIZE;
        i++;
    }
    for(uint64_t j = 0; j < i; j++) {
        if(!block[j].is_free) {
            return NULL;
        }
    }
    return (void *)(start_addr + (block[0].index * BLOCK_SIZE));
}

void kfree(void *addr) {
    Block block = head;
    while(!block.is_free && block.node.next != NULL) {
        block = *(Block *)block.node.next;
        if((void *)(start_addr + (block.index * BLOCK_SIZE)) == addr) {
            block.is_free = true;
        }
    }
}