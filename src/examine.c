#include "commands.h"
#include "expr.h"
#include "cmdutils.h"
#include "disass.h"

int CMD(examine, "reads a {string(s),digit(d),instruction(i)} from [expr]",
        s_proc *proc, int argc, char *argv[])
{
    if (argc < 3)
        CMD_FAIL("usage: {s,d,i} [expr]\n");

    t_ureg addr;
    if (proc_expr_eval(proc, argv[2], &addr))
        return CMD_FAILURE;

    switch (argv[1][0])
    {
    case 'x':
    case 'd':
    case 's':
        puts("not implemented");
        return CMD_FAILURE;
    case 'i':
        return print_disass(proc, addr, argv[3]) * CMD_FAILURE;
    default:
        fprintf(stderr, "invalid format \"%s\"\n", argv[1]);
        return CMD_FAILURE;
    }
    return CMD_SUCCESS;
}
