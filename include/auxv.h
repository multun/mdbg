#pragma once

#include "process.h"

#include <stdbool.h>


/**
** \brief retrieves some values from a process's auxv
** \desc if no correspinding key is found, 0 is stored in *value
** \param proc the process to query
** \param key the auxv key
** \param value an address to store the result in
** \return whether the operation failed
*/
bool proc_auxv_get(s_proc *proc, unsigned long key, unsigned long *value);
