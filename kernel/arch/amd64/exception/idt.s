.intel_syntax noprefix
.altmacro

.section .data

idtr: 
  .word 0
  .long 0

.section .text

.macro pushaq
  push rax
  push rbx
  push rcx
  push rdx
  push rbp
  push rdi
  push rsi
  push r8
  push r9
  push r10
  push r11
  push r12
  push r13
  push r14
  push r15
.endm

.macro popaq
  pop r15
  pop r14
  pop r13
  pop r12
  pop r11
  pop r10
  pop r9
  pop r8
  pop rsi
  pop rdi
  pop rbp
  pop rdx
  pop rcx
  pop rbx
  pop rax
.endm

.global idt_load
.type idt_load, @function

idt_load:
  cli
  mov idtr, di
  mov idtr+2, rsi
  lidt idtr
  sti
  ret

.global exc_get_error
.type exc_get_error, @function

exc_get_error:
  pop rax
  push rax
  ret

.global exc_get_cr2
.type exc_get_cr2, @function

exc_get_cr2:
  mov rax, cr2
  ret

.global exc_get_rip
.type exc_get_rip, @function

exc_get_rip:
  mov rax, qword ptr [rsp + 4]
  ret

.macro interrupt_handler x
.global interrupt_\x
.type interrupt_\x, @function

interrupt_\x:
  cld
  pushaq
  mov di, \x
  mov rsi, cr2
  call interrupt
  popaq
  iretq
.endm

interrupt_handler 0x0  # EXC_DIVISION
interrupt_handler 0x1  # EXC_DEBUG
interrupt_handler 0x2  # EXC_NM_INTERRUPT
interrupt_handler 0x3  # EXC_BREAKPOINT
interrupt_handler 0x4  # EXC_OVERFLOW
interrupt_handler 0x5  # EXC_RANGE_EXCEEDED
interrupt_handler 0x6  # EXC_INVALID_OPCODE
interrupt_handler 0x7  # EXC_DEVICE_NOT_AVAILABLE
interrupt_handler 0x8  # EXC_DOUBLE_FAULT
interrupt_handler 0x9  # EXC_SEGMENT_OVERRUN
interrupt_handler 0xA  # EXC_INVALID_TSS
interrupt_handler 0xB  # EXC_SEG_NOT_PRESENT
interrupt_handler 0xC  # EXC_SEG_FAULT_STACK
interrupt_handler 0xD  # EXC_GP_FAULT
interrupt_handler 0xE  # EXC_PAGE_FAULT
interrupt_handler 0x10 # EXC_FLOATING_POINT
interrupt_handler 0x11 # EXC_ALIGNMENT_CHECK
interrupt_handler 0x12 # EXC_MACHINE_CHECK 
interrupt_handler 0x13 # EXC_SIMD
interrupt_handler 0x14 # EXC_VIRTUALIZATION
interrupt_handler 0x15 # EXC_CONTROL_PROTECTION
interrupt_handler 0x1C # EXC_HYPERVISOR_INJECT
interrupt_handler 0x1D # EXC_VMM_COMMUNICATION
interrupt_handler 0x1E # EXC_SECURITY
interrupt_handler 0x80 # EXC_SYSCALL
