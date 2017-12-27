#include "proc_trace.h"

#include <errno.h>
#include <stddef.h>


bool proc_trace(s_proc *proc, enum __ptrace_request request,
                void *addr, void *data)
{
    int res = ptrace(request, proc->pid, addr, data);
    if (res < 0)
    {
        if (errno != ESRCH)
            return true;
        proc->ev = PROC_EXITED;
        proc->exit_status = 1;
    }
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


bool proc_cont(s_proc *proc, enum __ptrace_request request)
{
    int sig = proc->deliver_signal ? proc->signal : 0;
    return proc_sigtrace(proc, PTRACE_SYSCALL, sig);
}
