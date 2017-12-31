mdbglib_sources = ['mdbg.c', 'tracer.c', 'tracee.c',
    'process.c', 'setup.c', 'proc_trace.c', 'alloc.c',
    'mlist.c', 'mvect.c', 'procfs.c', 'auxv.c', 'breakpoint.c',
    'commands.c', 'info_memory.c', 'proc_wait.c', 'continue.c',
    'breaks.c', 'autocomplete.c', 'info_sig.c', 'info_regs.c',
    'proc_regs.c', 'setreg.c']


cc = meson.get_compiler('c')
readline_dep = cc.find_library('readline', required: true)

mdbglib = shared_library('mdbg',
                         mdbglib_sources,
                         include_directories : inc,
                         install : true,
                         dependencies: [readline_dep])
