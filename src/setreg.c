#include "commands.h"
#include "proc_regs.h"
#include "proc_trace.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int CMD(setreg, "sets the value of a register",
        s_proc *proc, int argc, char *argv[])
{
    if (argc < 3)
    {
        fprintf(stderr, "usage: <regname> <value>\n");
        return CMD_FAILURE;
    }

    e_uregs regid;
    for (size_t i = 0; i < UREG_COUNT; i++)
        if (!strcmp(argv[1], UREG_NAME(i)))
        {
            regid = i;
            goto found_id;
        }
    fprintf(stderr, "unknown register \"%s\"\n", argv[1]);
    return CMD_FAILURE;

    char *endptr;
    // labels to declarations aren't allowed :(
found_id:
    endptr = NULL;
    t_ureg reg = strtoull(argv[2], &endptr, 0);
    if (endptr && *endptr)
    {
        fprintf(stderr, "invalid digit: %c", *endptr);
        return CMD_FAILURE;
    }

    return proc_setreg(proc, regid, reg) * CMD_FAILURE;
}
