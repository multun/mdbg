#include "commands.h"
#include "proc_regs.h"
#include "proc_trace.h"

#include <stdio.h>


int CMD(info_regs, "prints details about the program's registers",
        s_proc *proc, UNUSED int argc, UNUSED char *argv[])
{
    struct user_regs_struct ur;
    if (proc_trace(proc, PTRACE_GETREGS, NULL, &ur))
        return CMD_FAILURE;

    for (size_t i = 0; i < UREG_COUNT; i++)
        fprintf(stderr, "%-15s\t0x%llx\n", UREG_NAME(i), UREG(&ur, i));

    return CMD_SUCCESS;
}
