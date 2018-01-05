#include "cmdutils.h"
#include "expr.h"

#include <stdio.h>


static t_ureg proc_expr_eval_reg(s_proc *proc, const char *expr)
{
    e_uregs regid = proc_findreg(expr + 1);
    if (regid == UREG_COUNT)
    {
        fprintf(stderr, "no register called \"%s\"", expr);
        return -1;
    }

    t_ureg res;
    if (proc_getreg(proc, regid, &res))
        return -1;

    return res;
}


t_ureg proc_expr_eval(s_proc *proc, const char *expr)
{
    if (expr[0] == '%')
        return proc_expr_eval_reg(proc, expr + 1);

    size_t res;
    return parse_size_t(expr, &res) ? -1 : res;
}
