#pragma once

#include "process.h"

#include <elf.h>

#define SYM_BAD (Elf64_Addr)-1

/**
** \brief looks up symname into a process image
** \param proc the process to lookup into
** \param symname the name of the symbol to lookup
** \return the address of the symbol if found, -1 otherwise
*/
Elf64_Addr proc_lookup_symbol(s_proc *proc, const char *symname);
