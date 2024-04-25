subdir('arch/amd64')

kernel_sources = files([
  'kernel.c',
  'video/framebuffer.c',
  'terminal/terminal.c',
  'terminal/log.c',
  'terminal/psf.c',
  'exception/panic.c',
  'sync/lock.c',
  'memory/pmm.c',
  'memory/vmm.c',
]) + arch_sources
