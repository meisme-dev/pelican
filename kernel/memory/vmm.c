#include "vmm.h"
#include "memory/pmm.h"
#include "sync/lock.h"

page_header_t *vmm_alloc_mem(uint64_t size) {
  static atomic_flag lock = ATOMIC_FLAG_INIT;
  acquire(&lock);
  page_header_t *current_page = pmm_alloc_page();  
  if(!current_page) {
    release(&lock);
    return NULL;
  }
  for(uint64_t i = 0; i < size / PAGE_SIZE; i++) {
   while(current_page->next) {
      current_page = current_page->next;
    }
    page_header_t *new_page = pmm_alloc_page();
    if(!new_page) {
     break;
    }
   current_page->next = new_page; 
  }  
  release(&lock);
  return NULL;
}

void vmm_free_mem(page_header_t *header) {
  if(header == NULL) {
    return;
  }
  static atomic_flag lock = ATOMIC_FLAG_INIT; 
  acquire(&lock);
  page_header_t *current_header = header, *prev_header = header;
  while(current_header->next) {
    prev_header = current_header;
    current_header = current_header->next;
    pmm_free_page(prev_header);
  }
  release(&lock); 
}

