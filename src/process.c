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


static bool proc_stopped_status(s_proc *proc, e_proc_ev *res, int signal)
{
    switch (signal)
    {
    case SIGTRAP | 0x80:
        *res = PROC_SYSCALL;
        break;

    case SIGTRAP:
        {
            siginfo_t si;
            if (proc_trace(proc, PTRACE_GETSIGINFO, NULL, &si))
                return true;
            printf("%d\n", si.si_code);
            switch (si.si_code)
            {
            // see TRAP_TRACE in siginfo.h
            case 2:
                *res = PROC_SINGLESTEP;
                break;

            // signal sent by the kernel, see man 2 ptrace
            case 0x80:
                *res = PROC_BREAKPOINT;
                break;

            default:
                if (proc->setup_done)
                    // this is a signal sent by the user
                    *res = PROC_STOPPED;
                else
                    // the kernel initialy stops the process
                    // in a kind of stange way, that appears to come
                    // from userland
                    *res = PROC_SINGLESTEP;
                break;
            }
            break;
        }

    default:
        *res = PROC_STOPPED;
        break;
    }
    return false;
}


static bool proc_update_status(s_proc *proc, int status)
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
        if (proc_stopped_status(proc, &proc->ev, WSTOPSIG(status)))
            return true;
        proc->signal = proc->ev == PROC_STOPPED ? WSTOPSIG(status) : 0;
    }
    else if (WIFCONTINUED(status))
        proc->ev = PROC_CONTINUED;
    else
        abort();

    proc->deliver_signal = (proc->ev == PROC_STOPPED);
    return false;
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
    return (proc_update_status(proc, status)
            || proc_ensure_setup(proc));
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
    case PROC_BREAKPOINT:
        warnx("hit a breakpoint");
        break;
    case PROC_SINGLESTEP:
        warnx("singlestep");
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
