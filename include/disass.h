#pragma once

#include "process.h"

#include <capstone/capstone.h>
#include <stddef.h>


cs_insn *disass(s_proc *proc, long addr, size_t *isize);
