#include "auxv.h"
#include "breakpoint.h"
#include "cmdutils.h"
#include "commands.h"
#include "err.h"

#include <elf.h>
#include <stdlib.h>


int CMD(break, "adds a breakpoint at a specified address",
        s_proc *proc, int argc, char *argv[])
{
    if (argc != 2)
    {
        warnx("usage: %s [addr]", *argv);
        return CMD_FAILURE;
    }

    size_t addr;
    return ((parse_size_t(argv[1], &addr)
             || proc_add_breakpoint(proc, (void*)addr, true))
            * CMD_FAILURE);
}
