#pragma once

#include "proc_regs.h"

#include <stdbool.h>
#include <stddef.h>


/**
** \brief parses an integer as big as an user register
** \param str the string to parse
** \param res where to store the result
** \return whether the operation failed
*/
bool parse_ureg(const char *str, t_ureg *res);
