#include "proc_regs.h"
#include "proc_trace.h"

#include <sys/user.h>


s_ureg_meta g_ureg_meta[UREG_COUNT] =
{
#define RMETA(EName, Name) { #Name, offsetof(struct user_regs_struct, Name) },
    UREGS_APPLY(RMETA)
#undef RMETA
};


bool proc_getreg(s_proc *proc, e_uregs regid, t_ureg *res)
{
    struct user_regs_struct ur;
    if (proc_trace(proc, PTRACE_GETREGS, NULL, &ur))
        return true;

    *res = UREG(&ur, regid);
    return false;
}


bool proc_setreg(s_proc *proc, e_uregs regid, t_ureg reg)
{
    struct user_regs_struct ur;
    if (proc_trace(proc, PTRACE_GETREGS, NULL, &ur))
        return true;

    UREG(&ur, regid) = reg;
    return proc_trace(proc, PTRACE_SETREGS, NULL, &ur);
}
