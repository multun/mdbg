#pragma once

#include "proc_regs.h"


/**
** \desc evaluates an expression, which may be a number as accepted by strtoul
**   or a register name starting %
** \param proc the process to evaluate the expression in
** \param expr the expression string to evaluate
*/
t_ureg proc_expr_eval(s_proc *proc, const char *expr);
