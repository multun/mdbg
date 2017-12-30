#include <elf.h>
#include <err.h>
#include <stdbool.h>
#include <stdio.h>
#include <sys/ptrace.h>
#include <sys/wait.h>
#include <sys/wait.h>
#include <unistd.h>

#include "auxv.h"
#include "breakpoint.h"
#include "commands.h"
#include "proc_trace.h"
#include "process.h"
#include "setup.h"

#include <readline/readline.h>


static void proc_continue(s_proc *proc)
{
    if (proc_cont(proc))
        errx(1, "continuing failed");
}


int tracer(int child_pid)
{
    printf("debugging PID %d\n", child_pid);
    s_proc child = PROC(child_pid);
    while (true)
    {
        int status;
        int signal_pid = waitpid(child_pid, &status, __WALL);
        if (signal_pid < 0)
            err(1, "waitpid failed");

        unsigned long ep;
        if (proc_auxv_get(&child, AT_ENTRY, &ep))
            warnx("auxv_get failed");
        else
            printf("ep: %lx\n", ep);
        proc_update(&child, status);
        proc_describe(&child);
        if (child.ev == PROC_TRAPPED)
            printf("[%d]\n", cmd_parserun(&child, readline(">")));

        if (!PROC_ALIVE(&child))
            break;

        proc_add_breakpoint(&child, (void*)ep);
        proc_continue(&child);
    }
    puts("i'm done here");
    return 0;
}
