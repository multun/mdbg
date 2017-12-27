#include "setup.h"
#include "proc_trace.h"

#include <sys/ptrace.h>


#define TRACEE_BASE_FLAGS PTRACE_O_TRACESYSGOOD

#ifdef PTRACE_O_EXITKILL
#define TRACEE_FULL_FLAGS (PTRACE_O_EXITKILL | TRACEE_BASE_FLAGS)
#else
#define TRACEE_FULL_FLAGS TRACEE_BASE_FLAGS
#endif


bool tracee_setup(int child)
{
    return ptrace(PTRACE_SETOPTIONS, child, 0, TRACEE_FULL_FLAGS);
}
