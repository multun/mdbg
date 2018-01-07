project('mdbg', 'c',
        version: '0.1',
        license: 'MIT')

inc = include_directories('include')
subdir('src')

testexe = executable('mdbg', 'tracer.c',
                     include_directories: inc,
                     link_with: mdbglib)
