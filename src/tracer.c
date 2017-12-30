#include <elf.h>
#include <err.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ptrace.h>
#include <unistd.h>

#include "autocomplete.h"
#include "auxv.h"
#include "breakpoint.h"
#include "commands.h"
#include "proc_trace.h"
#include "process.h"

#include <readline/readline.h>
#include <readline/history.h>


static void proc_continue(s_proc *proc)
{
    if (proc_cont(proc))
        errx(1, "continuing failed");
}


static int interract(s_proc *proc)
{
    int status = 0;
    do {
        char *line = readline("> ");
        status = cmd_parserun(proc, line);
        if (!(status & CMD_NOT_FOUND))
            add_history(line);
        free(line);
    } while (!(status & (CMD_CONT | CMD_EXIT)));
    return status;
}


int tracer(int child_pid)
{
    autocomplete_setup();
    printf("debugging PID %d\n", child_pid);
    s_proc child = PROC(child_pid);

    int cmd_status = 0;

    goto tracer_init;

    do {
        /* unsigned long ep; */
        /* if (proc_auxv_get(&child, AT_ENTRY, &ep)) */
        /*     warnx("auxv_get failed"); */
        /* else */
        /*     printf("ep: %lx\n", ep); */
        /* proc_add_breakpoint(&child, (void*)ep); */

        // TODO: error handling
        proc_continue(&child);
    tracer_init:
        proc_wait(&child);

        if (!proc_is_userstop(&child))
            continue;

        proc_describe(&child);
        cmd_status = interract(&child);
    } while (PROC_ALIVE(&child) && !(cmd_status & CMD_EXIT));

    puts("i'm done here");
    return CMD_SUCCEEDED(cmd_status);
}
