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

regular_font_lib = static_library('regular_font', regular_font_target)
regular_font_dep = declare_dependency(link_with: regular_font_lib)

bold_font_lib = static_library('bold_font', bold_font_target)
bold_font_dep = declare_dependency(link_with: bold_font_lib)

executable('pelican', sources: sources, dependencies: [bold_font_dep, regular_font_dep], link_args: link_args, include_directories: include_directories)