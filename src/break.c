#include "auxv.h"
#include "breakpoint.h"
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

    char *endptr = NULL;
    unsigned long int addr = strtoul(argv[1], &endptr, 0);
    if (endptr && *endptr)
    {
        fprintf(stderr, "invalid digit: %c", *endptr);
        return CMD_FAILURE;
    }

    return proc_add_breakpoint(proc, (void*)addr, true) * CMD_FAILURE;
}
