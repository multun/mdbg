#pragma once

#include "proc_regs.h"

#include <stdbool.h>
#include <stddef.h>


bool parse_ureg(const char *str, t_ureg *res);


#define CMD_FAIL(...)                  \
    do {                               \
        fprintf(stderr, __VA_ARGS__);  \
        return CMD_FAILURE;            \
    } while (0);
