#include "breakpoint.h"
#include "commands.h"
#include "proc_trace.h"


int CMD(stepi, "steps to the next instruction",
        s_proc *proc, int argc, char *argv[])
{
    return (proc->ev == PROC_BREAKPOINT
            ? proc_breakpoint_step
            : proc_singlestep)(proc) * CMD_FAILURE;
}
