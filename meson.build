project('pelican', 'c')

subdir('kernel')
subdir('libk')
subdir('assets')

link_args = [
  '-Wl,-T',
  '-Wl,' + meson.source_root() + '/linker.ld'
]

sources = [ 
  kernel_sources,
  libk_sources
]

include_directories = include_directories('libk/include', 'kernel/include', 'vendor')

font_lib = static_library('assets', font_target)
font_dep = declare_dependency(link_with: font_lib)

executable('pelican', sources: sources, dependencies: font_dep, link_args: link_args, include_directories: include_directories)