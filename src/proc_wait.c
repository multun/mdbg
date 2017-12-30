#include "process.h"

#include <err.h>
#include <errno.h>
#include <sys/wait.h>


bool proc_wait(s_proc *proc)
{
    int status;
    int signal_pid;
    while ((signal_pid = waitpid(proc->pid, &status, __WALL)) < 0
           && errno == EINTR)
        continue;

    if (signal_pid < 0)
    {
        warn("waitpid failed");
        return true;
    }

    return proc_update(proc, status);
}
