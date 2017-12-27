#pragma once

#include "process.h"

#include <stdbool.h>
#include <sys/ptrace.h>


bool proc_cont(s_proc *proc, enum __ptrace_request request);
bool proc_sigtrace(s_proc *proc, enum __ptrace_request request, int sig);
bool proc_trace(s_proc *proc, enum __ptrace_request request,
                void *addr, void *data);
