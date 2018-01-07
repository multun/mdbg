#include <signal.h>

#include "commands.h"
#include "proc_trace.h"
#include "macro_utils.h"

#include <stddef.h>
#include <stdio.h>


int CMD(info_sig, "prints details about the stopping signal",
        s_proc *proc, UNUSED int argc, UNUSED char *argv[])
{
    siginfo_t si;
    if (proc_trace(proc, PTRACE_GETSIGINFO, NULL, &si))
        return CMD_FAILURE;

    struct
    {
        const char *name;
        size_t off;
    } fields[] =
    {
#define FIELD(Name) { #Name, offsetof(siginfo_t, Name) }
        FIELD(si_signo),
        FIELD(si_errno),
        FIELD(si_code),
#undef FIELD
    };

    for (size_t i = 0; i < ARR_SIZE(fields); i++)
        fprintf(stderr, "%s\t%d\n", fields[i].name,
                *(int*)((char*)&si + fields[i].off));
    psiginfo(&si, "psiginfo");
    return CMD_SUCCESS;
}
