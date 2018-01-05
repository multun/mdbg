#include "cmdutils.h"

#include <stdio.h>
#include <stdlib.h>


bool parse_size_t(const char *str, size_t *res)
{
    char *endptr = NULL;
    *res = strtoull(str, &endptr, 0);
    if (!(endptr && *endptr))
        return false;

    fprintf(stderr, "invalid digit: %c", *endptr);
    return true;
}
