#include "auxv.h"
#include "breakpoint.h"
#include "commands.h"
#include "err.h"
#include "expr.h"

#include <elf.h>
#include <stdlib.h>

#define USAGE "[expr]"

static int break_sub(s_proc *proc, int argc, char *argv[], bool persistant)
{
    if (argc != 2)
    {
        CMD_USAGE(USAGE, argv);
        return CMD_FAILURE;
    }

    t_ureg addr;
    return ((proc_expr_eval(proc, argv[1], &addr)
             || proc_add_breakpoint(proc, (void*)addr, persistant))
            * CMD_FAILURE);
}


int CMD(break, "adds a breakpoint at a specified expression",
        s_proc *proc, int argc, char *argv[])
{
    return break_sub(proc, argc, argv, true);
}




int CMD(tbreak, "adds a temporary breakpoint at a specified expression",
        s_proc *proc, int argc, char *argv[])
{
    return break_sub(proc, argc, argv, false);
}
