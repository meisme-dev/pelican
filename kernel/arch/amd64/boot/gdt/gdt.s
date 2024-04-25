.intel_syntax noprefix

.section .data

gdtr: 
  .word 0
  .long 0

.section .text
.global gdt_load
.global gdt_segments_reload

.type gdt_load, @function
.type gdt_segments_reload, @function

gdt_load:
  cli
  mov gdtr, di
  mov gdtr+2, rsi
  lgdt gdtr
  ltr dx
  ret

gdt_segments_reload:
  push 0x8
  lea rax, [rip + .cs_reload]
  push rax
  retfq

.cs_reload:
  mov ax, 0x10
  mov ds, ax
  mov es, ax
  mov fs, ax
  mov fs, ax
  mov ss, ax
  ret
