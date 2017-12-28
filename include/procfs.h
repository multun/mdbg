#pragma once

#include "process.h"


/**
** \brief opens a file at /proc/<PID>/<FNAME>
** \param proc the process to explore
** \param fname the name of the procfs entry to open
** \param flags the flags to pass to open()
** \return the resulting file descriptor
*/
int proc_open_procfs(s_proc *proc, const char *fname, int flags);
