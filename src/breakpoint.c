#include "alloc.h"
#include "breakpoint.h"
#include "commands.h"
#include "proc_regs.h"
#include "proc_trace.h"

#include <err.h>


bool proc_breakpoint_enable(s_proc *proc, s_breakpoint *bp)
{
    long data;
    if (proc_peek(proc, bp->addr, &data))
        return true;

    data &= ~0xFFUL;
    data |= 0xCC;

    return proc_poke(proc, bp->addr, data);
}


bool proc_breakpoint_disable(s_proc *proc, s_breakpoint *bp)
{
    long data;
    if (proc_peek(proc, bp->addr, &data))
        return true;

    data &= ~0xFFUL;
    data |= bp->orig_data;

    return proc_poke(proc, bp->addr, data);
}


bool proc_breakpoint_disable_all(s_proc *proc)
{
    MLIST_FOREACH(cur, BPLIST, &proc->breakpoints)
        if (proc_breakpoint_disable(proc, cur))
            return true;
    return false;
}


bool proc_breakpoint_enable_all(s_proc *proc)
{
    MLIST_FOREACH(cur, BPLIST, &proc->breakpoints)
        if (proc_breakpoint_enable(proc, cur))
            return true;
    return false;
}



bool proc_add_breakpoint(s_proc *proc, void *addr, bool persistant)
{
    s_breakpoint tmp_bp = BREAKPOINT(addr, persistant);

    long data;
    if (proc_peek(proc, addr, &data))
        return true;

    tmp_bp.orig_data = data & 0xFFUL;

    if (proc_breakpoint_enable(proc, &tmp_bp))
        return true;

    s_breakpoint *bp = xmalloc(sizeof(*bp));
    *bp = tmp_bp;
    MLIST_ADD(BPLIST, &proc->breakpoints, bp);
    return false;
}


static bool breakpoint_equal(void *bp, void *addr)
{
    return ((s_breakpoint*)bp)->addr == addr;
}


static s_breakpoint *proc_find_breakpoint(s_proc *proc, void *addr)
{
    return MLIST_FIND(BPLIST, &proc->breakpoints, breakpoint_equal, addr);
}


bool proc_breakpoint_prepare(s_proc *proc)
{
    t_ureg rip;

    if (proc_getreg(proc, UREG_RIP, &rip))
        return true;

    rip--;
    // only decrement RIP if a matching breakpoint is
    // found we cannot keep a reference to the breakpoint
    // inside proc as it may be deleted by the user
    if (!(proc_find_breakpoint(proc, (void*)rip)))
    {
        fprintf(stderr, "unregistered breakpoint at 0x%llx", rip);
        return false;
    }

    return proc_setreg(proc, UREG_RIP, rip);
}


bool proc_breakpoint_step(s_proc *proc)
{
    t_ureg rip;
    if (proc_getreg(proc, UREG_RIP, &rip))
        return true;

    s_breakpoint *bp = proc_find_breakpoint(proc, (void*)rip);
    if (!bp)
        // the breakpoint may have been removed by the user
        return false;

    if (proc_breakpoint_disable(proc, bp)
        || proc_singlestep(proc)
        || proc_wait(proc)
        || proc->ev != PROC_SINGLESTEP)
        return true;

    if (bp->persistant)
        return false;

    MLIST_REMOVE(BPLIST, &proc->breakpoints, bp);
    return proc_breakpoint_disable(proc, bp);
}


int CMD(bp_step, "steps over a breakpoint",
        s_proc *proc, int argc, char *argv[])
{
    return proc_breakpoint_step(proc) * CMD_FAILURE;
}
