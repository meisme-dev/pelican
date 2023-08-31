.intel_syntax noprefix

.section text

.global cpu_rdmsr
.type cpu_rdmsr, @function
cpu_rdmsr:
  mov ecx, edi
  rdmsr
  shl rax, 32
  or rax, rdx
  ret

.global cpu_wrmsr
.type cpu_wrmsr, @function
cpu_wrmsr:
  mov eax, edi
  rol rdi, 32
  mov ecx, edi
  wrmsr
  ret
