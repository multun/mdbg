#include "auxv.h"
#include "breakpoint.h"
#include "commands.h"
#include "err.h"

#include <elf.h>


int CMD(b_ep, "adds a breakpoint at the program's entry point",
        s_proc *proc, int argc, char *argv[])
{
    unsigned long ep;
    if (proc_auxv_get(proc, AT_ENTRY, &ep))
    {
        warnx("auxv_get failed");
        return CMD_FAILURE;
    }

    printf("ep: %lx\n", ep);
    return proc_add_breakpoint(proc, (void*)ep, false) * CMD_FAILURE;
}
