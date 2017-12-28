#include "procfs.h"

#include <err.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>


int proc_open_procfs(s_proc *proc, const char *fname, int flags)
{
    const char path_format[] = "/proc/%d/%s";
    // each byte needs at most 3 chars to be represented
    // the +1 is here just in case its negative, eventhough it
    // should never be
    size_t format_comp_size = sizeof(int) * 3 + 1 + strlen(fname);
    char path[sizeof(path_format) + format_comp_size];
    sprintf(path, path_format, proc->pid, fname);

    int fd = open(path, flags);
    if (fd < 0)
        warn("couldn't open %s", path);
    return fd;
}
