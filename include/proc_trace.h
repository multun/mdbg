#pragma once

#include "process.h"

#include <stdbool.h>
#include <sys/ptrace.h>


bool proc_cont(s_proc *proc);
bool proc_sigtrace(s_proc *proc, enum __ptrace_request request, int sig);
bool proc_trace(s_proc *proc, enum __ptrace_request request,
                void *addr, void *data);
bool proc_peek(s_proc *proc, void *addr, long *res);
bool proc_poke(s_proc *proc, void *addr, long data);
bool proc_singlestep(s_proc *proc);
