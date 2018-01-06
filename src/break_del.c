#include "breakpoint.h"
#include "commands.h"
#include "process.h"
#include "expr.h"


typedef bool (*f_mlist_pred)(void *elem, void *data);


static bool break_has_addr(void *bp, void *addr)
{
    return ((s_breakpoint*)bp)->addr == addr;
}


int CMD(break_del, "delete a breakpoint",
        s_proc *proc, int argc, char *argv[])
{
    if (argc != 2)
    {
        fprintf(stderr, "Usage: %s (id|[expr])\n", argv[0]);
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
