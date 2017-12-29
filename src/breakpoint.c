#include "breakpoint.h"
#include "proc_trace.h"


bool proc_add_breakpoint(s_proc *proc, void *addr)
{
    int data;
    if (proc_peek(proc, addr, &data))
        return true;

    data &= 0xFFFFFF;
    data |= 0xCC << 24;
    return proc_poke(proc, addr, data);
}
