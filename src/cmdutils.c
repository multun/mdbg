#include "cmdutils.h"
#include "proc_regs.h"

#include <stdio.h>
#include <stdlib.h>


bool parse_ureg(const char *str, t_ureg *res)
{
    char *endptr = NULL;
    *res = strtoull(str, &endptr, 0);
    if (!(endptr && *endptr))
        return false;

    fprintf(stderr, "invalid digit: %c", *endptr);
    return true;
}
