#include "auxv.h"
#include "breakpoint.h"
#include "cmdutils.h"
#include "commands.h"
#include "disass.h"
#include "err.h"
#include "mvect.h"
#include "proc_regs.h"
#include "proc_trace.h"

#include <inttypes.h>
#include <elf.h>
#include <stdlib.h>


static cs_insn *try_disass(void *buf, size_t buf_size, long addr, size_t *isize)
{
    csh handle;
    if (cs_open(CS_ARCH_X86, CS_MODE_64, &handle))
    {
        printf("failed to initialize capstone!\n");
        return NULL;
    }

    cs_insn *insn;
    size_t count = cs_disasm(handle, buf, buf_size, addr, 1, &insn);
    if (count && isize)
        *isize = insn->size;

    cs_close(&handle);
    return !count ? NULL : insn;
}


cs_insn *disass(s_proc *proc, long addr, size_t *isize)
{
    const size_t imax = 6;

    s_mvect v;
    mvect_init(&v, 16);

    size_t i = 0;
    cs_insn *res;
    do {
        if (i++ >= imax)
            goto failure;

        long data;
        if (proc_peek(proc, (void*)addr, &data))
            goto failure;

        MVECT_PUSH(&v, long, data);
    } while (!(res = try_disass(v.data, v.size, addr, isize)));

    free(v.data);
    return res;

failure:
    free(v.data);
    return NULL;
}


static void format_insn(cs_insn *insn)
{
    printf("0x%"PRIx64":\t%s\t\t%s\n",
           insn->address,
           insn->mnemonic,
           insn->op_str);
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

    t_ureg icount = 1;
    if (argc > 1 && parse_ureg(argv[1], &icount))
        return CMD_FAILURE;

    t_ureg rip;
    if (proc_getreg(proc, UREG_RIP, &rip))
        return CMD_FAILURE;


    proc_breakpoint_disable_all(proc);

    // disassembling 0 instructions is OK
    cs_insn *insn = (void*)-1;

    size_t acc = 0;
    for (size_t i = 0; i < icount; i++)
    {
        size_t cur_size;
        if (!(insn = disass(proc, rip + acc, &cur_size)))
            break;

        format_insn(insn);
        cs_free(insn, 1);
        acc += cur_size;
    }

    proc_breakpoint_enable_all(proc);
    return !insn * CMD_FAILURE;
}
