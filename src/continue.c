#include "commands.h"
#include "procfs.h"


int CMD(continue, "let the tracee run until a watched event occurs",
        UNUSED s_proc *proc, UNUSED int argc, UNUSED char *argv[])
{
    return CMD_CONT;
}
