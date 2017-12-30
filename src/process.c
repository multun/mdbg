#include "process.h"
#include "tracee.h"
#include "setup.h"
#include "proc_trace.h"

#include <sys/user.h>
#include <err.h>
#include <errno.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/ptrace.h>
#include <sys/wait.h>


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


static void proc_update_status(s_proc *proc, int status)
{
    proc->exit_status = 0;
    proc->deliver_signal = false;
    proc->signal = 0;

    if (WIFEXITED(status))
    {
        proc->ev = PROC_EXITED;
        proc->exit_status = WEXITSTATUS(status);
    }
    else if (WIFSIGNALED(status))
    {
        proc->ev = PROC_SIGNALED;
        proc->signal = WTERMSIG(status);
        proc->coredump = WCOREDUMP(status);
    }
    else if (WIFSTOPPED(status))
    {
        proc->signal = WSTOPSIG(status);
        proc->ev = proc_stopped_status(proc->signal);
    }
    else if (WIFCONTINUED(status))
        proc->ev = PROC_CONTINUED;
    else
        abort();

    proc->deliver_signal = (proc->ev == PROC_STOPPED);
}


static bool proc_ensure_setup(s_proc *proc)
{
    if (proc->setup_done)
        return false;

    proc->setup_done = true;
    return tracee_setup(proc->pid);
}



bool proc_update(s_proc *proc, int status)
{
    proc_update_status(proc, status);
    return proc_ensure_setup(proc);
}


static int proc_anal_syscall(s_proc *proc)
{
    struct user_regs_struct regs;

    if (proc_trace(proc, PTRACE_GETREGS, NULL, &regs))
        err(1, "GETREGS failed");

    return regs.orig_rax;
}


void proc_describe(s_proc *proc)
{
    switch (proc->ev)
    {
    case PROC_INIT:
        warnx("initializing");
        break;
    case PROC_EXITED:
        warnx("exited with status %d", proc->exit_status);
        break;
    case PROC_SIGNALED:
        warnx("killed with signal %d", proc->signal);
        break;
    case PROC_STOPPED:
        warnx("stopped with signal %d", proc->signal);
        break;
    case PROC_SYSCALL:
        warnx("inside syscall(%d)", proc_anal_syscall(proc));
        break;
    case PROC_TRAPPED:
        warnx("trapped");
        break;
    case PROC_CONTINUED:
        warnx("continued");
        break;
    }
}


bool proc_is_userstop(s_proc *proc)
{
    if (proc->ev != PROC_SYSCALL)
        return true;

    int current_syscall = proc_anal_syscall(proc);
    MVECT_FOREACH(&proc->watched_syscalls, int, cur)
        if (*cur == current_syscall)
            return true;

    return false;
}
