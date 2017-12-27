#include <err.h>
#include <stdbool.h>
#include <stdio.h>
#include <sys/ptrace.h>
#include <sys/wait.h>
#include <sys/wait.h>
#include <unistd.h>

#include "proc_trace.h"
#include "process.h"
#include "setup.h"


static void proc_continue(s_proc *proc)
{
    if (proc_cont(proc, PTRACE_SYSCALL))
        err(1, "ptrace(PTRACE_SYSCALL) failed");
}


int tracer(int child_pid)
{
    s_proc child = PROC(child_pid);
    while (true)
    {
        int status;
        int signal_pid = waitpid(child_pid, &status, __WALL);
        if (signal_pid < 0)
            err(1, "waitpid failed");

        proc_update(&child, status);
        proc_describe(&child);

        if (!proc_alive(&child))
            break;

        proc_continue(&child);
    }
    puts("i'm done here");
    return 0;
}
