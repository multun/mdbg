mdbglib_sources = ['mdbg.c', 'tracer.c', 'tracee.c',
    'process.c', 'setup.c', 'proc_trace.c', 'alloc.c',
    'mlist.c', 'mvect.c', 'procfs.c', 'auxv.c']

mdbglib = shared_library('mdbg',
                         mdbglib_sources,
                         include_directories : inc,
                         install : true)
