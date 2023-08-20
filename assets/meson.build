linker = meson.get_external_property('ld', 'ld')

regular_font_target = custom_target(
    'regular.o',
    output : 'regular.o',
    input : 'regular.psf',
    command : [linker, '-r', '-b', 'binary', '@INPUT@', '-o', '@OUTPUT@'],
)


bold_font_target = custom_target(
    'bold.o',
    output : 'bold.o',
    input : 'bold.psf',
    command : [linker, '-r', '-b', 'binary', '@INPUT@', '-o', '@OUTPUT@'],
)