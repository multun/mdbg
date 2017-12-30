#include "commands.h"

#include <err.h>
#include <stdlib.h>


int CMD(breaks, "break on some syscall",
        s_proc *proc, int argc, char *argv[])
{
    if (argc < 2)
    {
        warnx("missing syscall number");
        return CMD_FAILURE;
    }

    MVECT_PUSH(&proc->watched_syscalls, int, atoi(argv[1]));
    return CMD_SUCCESS;
}
