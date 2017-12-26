#include <err.h>
#include <stdbool.h>
#include <stdio.h>
#include <sys/ptrace.h>
#include <sys/user.h>
#include <sys/wait.h>
#include <sys/wait.h>
#include <unistd.h>

#include "process.h"


int print_syscall(pid_t cpid)
{
    struct user_regs_struct regs;

    if (ptrace(PTRACE_GETREGS, cpid, NULL, &regs))
        err(1, "GETREGS failed");

    printf("syscall(%lld)\n", regs.orig_rax);
    return 0;
}


int tracer(int child_pid)
{
    s_proc child = PROC(child_pid);
    ptrace(PTRACE_SETOPTIONS, child, 0, PTRACE_O_TRACESYSGOOD);
    while (true)
    {
        int status;
        int signal_pid = waitpid(-1, &status, 0);
        if (signal_pid < 0)
            err(1, "waitpid failed");

        if (signal_pid != child_pid)
        {
            warnx("untraced pid %d was signaled", child_pid);
            continue;
        }

        proc_update(&child, status);
        if (!proc_alive(&child))
            break;
        proc_describe(&child);
        int sig = child.ev == PROC_STOPPED ? child.ev_data : 0;
        if (ptrace(PTRACE_SYSCALL, child, NULL, sig) < 0)
            err(1, "ptrace(PTRACE_SYSCALL) failed");
    }
    return 0;
}
