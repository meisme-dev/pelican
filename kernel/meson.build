subdir('arch/amd64')

kernel_sources = files([
  'kernel.c',
  'video/framebuffer.c',
  'terminal/terminal.c',
  'terminal/log.c',
  'terminal/psf.c',
  'common/exception/panic.c',
  'memory/pmm.c',
]) + arch_sources
