#include "mvect.h"
#include "auxv.h"
#include "commands.h"
#include "err.h"
#include "proc_trace.h"
#include "proc_regs.h"

#include "breakpoint.h"
#include <inttypes.h>
#include <capstone/capstone.h>
#include <elf.h>
#include <stdlib.h>


static bool try_disass(void *buf, size_t buf_size, long addr, size_t *isize)
{
    csh handle;
    if (cs_open(CS_ARCH_X86, CS_MODE_64, &handle))
    {
        printf("ERROR: Failed to initialize engine!\n");
        return -1;
    }

    cs_insn *insn;
    size_t count = cs_disasm(handle, buf, buf_size, addr, 1, &insn);
    if (count)
    {
        printf("0x%"PRIx64":\t%s\t\t%s\n",
               insn->address,
               insn->mnemonic,
               insn->op_str);
        cs_free(insn, count);

        if (isize)
            *isize = insn->size;
    }

    cs_close(&handle);
    return count;
}


static bool disass(s_proc *proc, long addr, size_t *isize)
{
    const size_t imax = 6;

    s_mvect v;
    mvect_init(&v, 16);

    size_t i = 0;
    do {
        if (i++ >= imax)
            goto failure;

        long data;
        if (proc_peek(proc, (void*)addr, &data))
            goto failure;



        MVECT_PUSH(&v, long, data);
    } while (!try_disass(v.data, v.size, addr, isize));

    free(v.data);
    return false;

failure:
    free(v.data);
    return true;
}


static bool disable_breakpoints(s_proc *proc)
{
    MLIST_FOREACH(cur, BPLIST, &proc->breakpoints)
        if (proc_breakpoint_disable(proc, cur))
            return true;
    return false;
}


static bool enable_breakpoints(s_proc *proc)
{
    MLIST_FOREACH(cur, BPLIST, &proc->breakpoints)
        if (proc_breakpoint_enable(proc, cur))
            return true;
    return false;
}



int CMD(disass, "disassembles the next instruction",
        s_proc *proc, int argc, char *argv[])
{
    if (argc > 2)
    {
        fprintf(stderr, "too many arguments. Usage: %s [instruction count]\n",
                *argv);
        return CMD_FAILURE;
    }

    size_t icount = 1;
    if (argc > 1)
    {
        char *endptr = NULL;
        icount = strtoul(argv[1], &endptr, 0);
        if (endptr && *endptr)
        {
            fprintf(stderr, "invalid digit: %c", *endptr);
            return CMD_FAILURE;
        }
    }

    t_ureg rip;
    if (proc_getreg(proc, UREG_RIP, &rip))
        return CMD_FAILURE;


    disable_breakpoints(proc);

    size_t acc = 0;
    bool res = false;
    for (size_t i = 0; i < icount; i++)
    {
        size_t cur_size;
        if ((res = disass(proc, rip + acc, &cur_size)))
            break;
        acc += cur_size;
    }

    enable_breakpoints(proc);
    return res * CMD_FAILURE;
}
