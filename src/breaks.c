#include "commands.h"

#include <err.h>
#include <stdlib.h>

#define USAGE "[syscall number]"


int CMD(breaks, USAGE " break on some syscall",
        s_proc *proc, int argc, char *argv[])
{
    if (argc < 2)
    {
        CMD_USAGE(USAGE, argv);
        return CMD_FAILURE;
    }

    MVECT_PUSH(&proc->watched_syscalls, int, atoi(argv[1]));
    return CMD_SUCCESS;
}
