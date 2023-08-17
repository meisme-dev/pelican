linker = meson.get_external_property('ld', 'ld')

font_target = custom_target(
    'font.o',
    output : 'font.o',
    input : 'font.psf',
    command : [linker, '-r', '-b', 'binary', '@INPUT@', '-o', '@OUTPUT@'],
)
