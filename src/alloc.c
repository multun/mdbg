#include "alloc.h"

#include <stdlib.h>
#include <err.h>


void *xmalloc(size_t size)
{
    void *ret = malloc(size);
    if (!ret && size)
        err(1, "malloc() failed");
    return ret;
}


void *xrealloc(void *ptr, size_t size)
{
    void *ret = realloc(ptr, size);
    if (!ret && size)
         err(1, "realloc() failed");
    return ret;
}
