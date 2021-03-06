#include "breakpoint.h"
#include "commands.h"
#include "process.h"
#include "expr.h"


static bool break_has_addr(void *bp, void *addr)
{
    return ((s_breakpoint*)bp)->addr == addr;
}


#define USAGE "[id|address]"

int CMD(break_del, USAGE " delete a breakpoint",
        s_proc *proc, UNUSED int argc, UNUSED char *argv[])
{
    if (argc != 2)
    {
        CMD_USAGE(USAGE, argv);
        return CMD_FAILURE;
    }

    bool is_id = argv[1][0] != '%' && !(argv[1][0] && argv[1][1] == 'x');
    if (is_id)
        argv[1]++;

    t_ureg addr;
    if (proc_expr_eval(proc, argv[1], &addr))
        return CMD_FAILURE;

    s_breakpoint *bp = is_id
        ? MLIST_NTH(BPLIST, &proc->breakpoints, addr)
        : MLIST_FIND(BPLIST, &proc->breakpoints, break_has_addr, (void*)addr);

    if (!bp)
    {
        fprintf(stderr, "breakpoint not found\n");
        return CMD_FAILURE;
    }

    if (proc_breakpoint_disable(proc, bp))
        return CMD_FAILURE;

    MLIST_REMOVE(BPLIST, &proc->breakpoints, bp);
    return CMD_SUCCESS;
}
