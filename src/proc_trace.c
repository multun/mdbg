#include "breakpoint.h"
#include "proc_trace.h"

#include <errno.h>
#include <stddef.h>


static bool handle_ptrace_error(s_proc *proc, bool force)
{
    if (errno != ESRCH && !force)
        return true;

    proc->ev = PROC_EXITED;
    proc->exit_status = 1;
    return false;
}


bool proc_trace(s_proc *proc, enum __ptrace_request request,
                void *addr, void *data)
{
    long res = ptrace(request, proc->pid, addr, data);
    if (res < 0)
        return handle_ptrace_error(proc, false);
    return false;
}


bool proc_sigtrace(s_proc *proc, enum __ptrace_request request, int sig)
{
    union
    {
        int sig;
        void *data;
    } un;
    un.data = NULL;
    un.sig = sig;

    return proc_trace(proc, request, NULL, un.data);
}


static int proc_getsig(s_proc *proc)
{
    return proc->deliver_signal ? proc->signal : 0;
}


bool proc_cont(s_proc *proc)
{
    while (proc->ev == PROC_BREAKPOINT)
        if (proc_breakpoint_step(proc))
            return true;

    enum __ptrace_request request = PTRACE_CONT;

    bool has_syscall_watches = proc->watched_syscalls.size;
    if (has_syscall_watches)
        request = PTRACE_SYSCALL;

    return proc_sigtrace(proc, request, proc_getsig(proc));
}


bool proc_singlestep(s_proc *proc)
{
    return proc_sigtrace(proc, PTRACE_SINGLESTEP, proc_getsig(proc));
}


bool proc_peek(s_proc *proc, void *addr, long *res)
{
    errno = 0;
    *res = ptrace(PTRACE_PEEKTEXT, proc->pid, addr, NULL);
    if (!errno)
        return false;

    handle_ptrace_error(proc, true);
    return true;
}


bool proc_poke(s_proc *proc, void *addr, long data)
{
    return proc_trace(proc, PTRACE_POKETEXT, addr, (void*)data);
}
