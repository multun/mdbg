project('mdbg', 'c',
        version: '0.1',
        license: 'MIT')

add_global_arguments(['-D_GNU_SOURCE', '-D_POSIX_C_SOURCE=200809L'], language: 'c')

inc = include_directories('include')
subdir('src')


testexe = executable('mdbg', 'tracer.c',
                     include_directories: inc,
                     link_with: mdbglib)
