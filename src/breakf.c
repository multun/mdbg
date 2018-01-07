#include "auxv.h"
#include "breakpoint.h"
#include "commands.h"
#include "err.h"
#include "expr.h"
#include "func_resolve.h"

#include <elf.h>
#include <stdlib.h>


int CMD(breakf, "add a breakpoint at the entry point of a function",
        s_proc *proc, int argc, char *argv[])
{
    if (argc != 2)
    {
        warnx("usage: %s [addr]", *argv);
        return CMD_FAILURE;
    }

    Elf64_Addr addr = proc_lookup_symbol(proc, argv[1]);
    if (addr == -1)
        return CMD_FAILURE;

    return proc_add_breakpoint(proc, (void*)addr, true);
}
