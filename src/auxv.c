#include "auxv.h"
#include "procfs.h"
#include "commands.h"

#include <err.h>
#include <fcntl.h>
#include <limits.h>
#include <stdio.h>
#include <unistd.h>


bool proc_auxv_get(s_proc *proc, unsigned long key, unsigned long *value)
{
    int fd = proc_open_procfs(proc, "auxv", O_RDONLY);
    if (fd < 0)
        return true;

    *value = 0;
    unsigned long cur[2];
    ssize_t rd;
    while ((rd = read(fd, cur, sizeof(cur))) > 0)
        if (cur[0] == key)
        {
            *value = cur[1];
            break;
        }

    close(fd);

    if (rd >= 0)
        return false;

    warn("auxv read for process %d", proc->pid);
    return true;
}


int CMD(auxv, "prints the auxv",
        s_proc *proc, int argc, char *argv[])
{
    int fd = proc_open_procfs(proc, "auxv", O_RDONLY);
    if (fd < 0)
        return true;

    unsigned long cur[2];
    ssize_t rd;
    while ((rd = read(fd, cur, sizeof(cur))) > 0)
        printf("%lx\t%lx\n", cur[0], cur[1]);

    close(fd);

    if (rd >= 0)
        return false;

    warn("auxv read for process %d", proc->pid);
    return true;
}
