#pragma once

#include "process.h"

#define SYMNAME_MAX_SIZE 4096


/**
** \brief initializes the backtrace feature
** \param proc the process to initialize
*/
bool backtrace_init(s_proc *proc);


/**
** \brief formats the current program backtrace to stdout
** \param proc the process to print the backtrace of
*/
void proc_backtrace(s_proc *proc);
