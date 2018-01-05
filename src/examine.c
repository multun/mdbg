#include "cmdutils.h"
#include "commands.h"
#include "disass.h"
#include "expr.h"
#include "proc_trace.h"

#include <string.h>


static int print_word(s_proc *proc, const char *format, t_ureg addr)
{
    long data;
    if (proc_peek(proc, (void*)addr, &data))
        return CMD_FAILURE;

    printf(format, data);
    return CMD_SUCCESS;
}


static int print_string(s_proc *proc, t_ureg addr)
{
    union
    {
        long l;
        char c[sizeof(long) + 1];
    } data;

    data.c[sizeof(long)] = 0;

    do {
        if (proc_peek(proc, (void*)addr, &data.l))
            return CMD_FAILURE;
        fputs(data.c, stdout);
        addr += sizeof(long);
    } while (strlen(data.c) == sizeof(long));

    putchar('\n');
    return CMD_SUCCESS;
}


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
        return print_word(proc, "%lx\n", addr);
    case 'd':
        return print_word(proc, "%ld\n", addr);
    case 's':
        return print_string(proc, addr);
    case 'i':
        return print_disass(proc, addr, argv[3]) * CMD_FAILURE;
    default:
        fprintf(stderr, "invalid format \"%s\"\n", argv[1]);
        return CMD_FAILURE;
    }
    return CMD_SUCCESS;
}
