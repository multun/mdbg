#pragma once

#include <stddef.h>


/**
** \brief a malloc wrapper failing on error
*/
void *xmalloc(size_t size);


/**
** \brief a realloc wrapper failing on error
*/
void *xrealloc(void *ptr, size_t size);
