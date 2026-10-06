.intel_syntax noprefix

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

.global cpu_enable_sse
.type cpu_enable_sse, @function
cpu_enable_sse:
  mov rax, cr4
  bts rax, 9
  bts rax, 10
  mov cr4, rax
  ret

.global cpu_enable_avx
.type cpu_enable_avx, @function
cpu_enable_avx:
  push rax
  push rcx
  push rdx
  xor rcx, rcx
  xgetbv
  or eax, 7
  xsetbv
  pop rdx
  pop rcx
  pop rax
  ret