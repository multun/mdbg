#pragma once

#include <stdbool.h>


/**
** \brief sets up the proper ptrace options for a child
** \return whether the operation succeeded
*/
bool tracee_setup(int child);
