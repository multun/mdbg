project('mdbg', 'c',
        version: '0.1',
        license: 'MIT')

add_global_arguments(['-O0', '-g3'], language: 'c')

inc = include_directories('include')
subdir('src')
capstone_dep = dependency('capstone')

cc = meson.get_compiler('c')
readline_dep = cc.find_library('readline', required: true)

testexe = executable('mdbg', 'main.c',
                     include_directories: inc,
                     link_with: mdbglib,
                     dependencies: [readline_dep, capstone_dep])
