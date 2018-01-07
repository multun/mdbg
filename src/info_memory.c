#include "commands.h"
#include "procfs.h"


int CMD(info_memory, "prints the memory map of the tracee",
        s_proc *proc, UNUSED int argc, UNUSED char *argv[])
{
    return proc_cat_procfs(proc, "maps") * CMD_FAILURE;
}
