#include "breakpoint.h"
#include "commands.h"
#include "process.h"


int CMD(break_list, "lists breakpoints",
        s_proc *proc, UNUSED int argc, UNUSED char *argv[])
{
    size_t id = 0;
    MLIST_FOREACH(cur, BPLIST, &proc->breakpoints)
        printf("%lu %c\t%p\n",
               id++,
               cur->persistant ? ' ' : 't',
               cur->addr);
    return CMD_SUCCESS;
}
