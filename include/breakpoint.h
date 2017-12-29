#pragma once


#include "process.h"

#include <stdbool.h>

/**
** \brief adds a breakpoint
** \param proc the process to add a breakpoint into
** \param addr the address to break at
** \return whether the operation succeeded
*/
bool proc_add_breakpoint(s_proc *proc, void *addr);
