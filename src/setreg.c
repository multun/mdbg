#include "commands.h"
#include "expr.h"
#include "proc_regs.h"
#include "proc_trace.h"

#include <stdio.h>


int CMD(setreg, "sets the value of a register",
        s_proc *proc, int argc, char *argv[])
{
    if (argc < 3)
    {
        fprintf(stderr, "usage: <regname> <value>\n");
        return CMD_FAILURE;
    }

    t_ureg reg;
    e_uregs regid = proc_findreg(argv[1]);
    return ((regid == UREG_COUNT
             || proc_expr_eval(proc, argv[2], &reg)
             || proc_setreg(proc, regid, reg))
            * CMD_FAILURE);
}
