project('mdbg', 'c',
        version: '0.1',
        license: 'MIT')

add_global_arguments(['-O0', '-g3'], language: 'c')

inc = include_directories('include')
subdir('src')

testexe = executable('mdbg', 'tracer.c',
                     include_directories: inc,
                     link_with: mdbglib,
                     dependencies: [capstone_dep])
