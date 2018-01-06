#include "cmdutils.h"
#include "expr.h"

#include <stdio.h>


static bool proc_expr_eval_reg(s_proc *proc, const char *expr, t_ureg *res)
{
    e_uregs regid = proc_findreg(expr);
    if (regid == UREG_COUNT)
    {
        fprintf(stderr, "no register called \"%s\"", expr);
        return true;
    }

    return proc_getreg(proc, regid, res);
}


bool proc_expr_eval(s_proc *proc, const char *expr, t_ureg *res)
{
    if (expr[0] == '%')
        return proc_expr_eval_reg(proc, expr + 1, res);

    return parse_ureg(expr, res);
}
