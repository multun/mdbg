#include "process.h"

#include <err.h>
#include <stdlib.h>
#include <sys/wait.h>


bool proc_alive(s_proc *proc)
{
    switch (proc->ev)
    {
    case PROC_SIGNALED:
    case PROC_EXITED:
        return false;
    default:
        return true;
    }
}


static e_proc_ev proc_stopped_status(int signal)
{
    switch (signal)
    {
    case SIGTRAP:
        return PROC_TRAPPED;
    case SIGTRAP | 0x80:
        return PROC_SYSCALL;
    default:
        return PROC_STOPPED;
    }
}


void proc_update(s_proc *proc, int status)
{
    if (WIFEXITED(status))
    {
        proc->ev = PROC_EXITED;
        proc->ev_data = WEXITSTATUS(status);
    }
    else if (WIFSIGNALED(status))
    {
        proc->ev = PROC_SIGNALED;
        proc->ev_data = WTERMSIG(status);
        proc->coredump = WCOREDUMP(status);
    }
    else if (WIFSTOPPED(status))
    {
        proc->ev_data = WSTOPSIG(status);
        proc->ev = proc_stopped_status(proc->ev_data);
    }
    else if (WIFCONTINUED(status))
        proc->ev = PROC_CONTINUED;
    else
        abort();
}


void proc_describe(s_proc *proc)
{
    switch (proc->ev)
    {
    case PROC_INIT:
        warnx("initializing");
        break;
    case PROC_EXITED:
        warnx("exited with status %d", proc->ev_data);
        break;
    case PROC_SIGNALED:
        warnx("killed with signal %d", proc->ev_data);
        break;
    case PROC_STOPPED:
        warnx("stopped with signal %d", proc->ev_data);
        break;
    case PROC_SYSCALL:
        warnx("inside a syscall");
        break;
    case PROC_TRAPPED:
        warnx("trapped");
        break;
    case PROC_CONTINUED:
        warnx("continued");
        break;
    }
}
