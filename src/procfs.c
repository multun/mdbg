#include "procfs.h"

#include <err.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/sendfile.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>


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


bool proc_cat_procfs(s_proc *proc, const char *fname)
{
    int fd = proc_open_procfs(proc, fname, O_RDONLY);
    if (fd < 0)
        return true;

    const size_t step = 1UL << 16;


    // lets assume sendfile returning a short count means
    // something wrong happened, as procfs is "special"
    ssize_t written;
    while ((written = sendfile(STDERR_FILENO, fd, NULL, step)) == step)
        continue;

    close(fd);

    if (written >= 0)
        return false;

    warn("sendfile failed");
    return true;
}
