#include "commands.h"
#include "proc_trace.h"
#include "macro_utils.h"

#include <signal.h>
#include <stddef.h>
#include <stdio.h>
#include <sys/user.h>


int CMD(info_regs, "prints details about the program's registers",
        s_proc *proc, int argc, char *argv[])
{
    struct user_regs_struct ur;
    if (proc_trace(proc, PTRACE_GETREGS, NULL, &ur))
        return CMD_FAILURE;

    struct
    {
        const char *name;
        size_t off;
    } fields[] =
    {
#define FIELD(Name) { #Name, offsetof(struct user_regs_struct, Name) }
        FIELD(r15),
        FIELD(r14),
        FIELD(r13),
        FIELD(r12),
        FIELD(rbp),
        FIELD(rbx),
        FIELD(r11),
        FIELD(r10),
        FIELD(r9),
        FIELD(r8),
        FIELD(rax),
        FIELD(rcx),
        FIELD(rdx),
        FIELD(rsi),
        FIELD(rdi),
        FIELD(orig_rax),
        FIELD(rip),
        FIELD(cs),
        FIELD(eflags),
        FIELD(rsp),
        FIELD(ss),
        FIELD(fs_base),
        FIELD(gs_base),
        FIELD(ds),
        FIELD(es),
        FIELD(fs),
        FIELD(gs),
#undef FIELD
    };

    typedef unsigned long long int t_ur_type;
    for (size_t i = 0; i < ARR_SIZE(fields); i++)
        fprintf(stderr, "%-15s\t0x%llx\n", fields[i].name,
                *(t_ur_type*)((char*)&ur + fields[i].off));

    return CMD_SUCCESS;
}
