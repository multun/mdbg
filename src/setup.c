#include "setup.h"
#include "proc_trace.h"

#include <sys/ptrace.h>


// TODO: check for EXITKILL availability
#define TRACEE_FLAGS (PTRACE_O_EXITKILL | PTRACE_O_TRACESYSGOOD)


bool tracee_setup(int child)
{
    return ptrace(PTRACE_SETOPTIONS, child, 0, TRACEE_FLAGS);
}
