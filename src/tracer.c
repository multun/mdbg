#include <stdio.h>

#include "autocomplete.h"
#include "backtrace.h"
#include "commands.h"
#include "interract.h"
#include "proc_trace.h"
#include "process.h"


int tracer(int child_pid)
{
    autocomplete_setup();
    printf("debugging PID %d\n", child_pid);
    s_proc child = PROC(child_pid);
    backtrace_init(&child);

    int cmd_status = 0;

    goto tracer_init;

    do {
        proc_cont(&child);
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
