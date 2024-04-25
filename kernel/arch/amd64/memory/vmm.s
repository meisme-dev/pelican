.intel_syntax noprefix

.section .text

.global vmm_load
.type vmm_load, @function

vmm_load:
  mov cr3, rdi
  ret

