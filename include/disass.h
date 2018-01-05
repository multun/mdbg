#pragma once

#include "proc_regs.h"

#include <capstone/capstone.h>
#include <stddef.h>


cs_insn *disass(s_proc *proc, long addr, size_t *isize);
bool print_disass(s_proc *proc, t_ureg addr, char *scount);
