#pragma once

#include <stddef.h>


/**
** \brief represents a buffer holding externally stored data
*/
typedef struct
{
    char *data;
    size_t size;
} s_ebuf;
