#include "disass.h"
#include "commands.h"
#include "proc_regs.h"
#include "breakpoint.h"


int CMD(nexti, "puts a temporary breakpoint onto the next instruction",
        s_proc *proc, int argc, char *argv[])
{
    t_ureg rip;
    if (proc_getreg(proc, UREG_RIP, &rip))
        return CMD_FAILURE;

    // all breakpoints are disabled just in case we run over one
    proc_breakpoint_disable_all(proc);

    size_t insn_size;
    cs_insn *insn = disass(proc, rip, &insn_size);
    bool failure = !insn;

    if (!failure)
    {
        failure |= proc_add_breakpoint(proc, (void*)(rip + insn_size), false);
        cs_free(insn, 1);
    }

    proc_breakpoint_enable_all(proc);
    return failure ? CMD_FAILURE : CMD_CONT;
}
