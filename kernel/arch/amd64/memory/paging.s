.intel_syntax noprefix

.section .text

.global paging_load
.type paging_load, @function

paging_load:
  mov cr3, rdi
  ret

