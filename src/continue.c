#include "commands.h"
#include "procfs.h"


int CMD(continue, "let the tracee run until a watched event occurs",
        s_proc *proc, int argc, char *argv[])
{
    return CMD_CONT;
}
