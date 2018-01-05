#include "commands.h"
#include "elfparse.h"
#include "proc_bin_map.h"
#include "process.h"


bool proc_lookup_symbol(s_proc *proc, const char *symname)
{
    s_elfimg ei;
    if (proc_map_bin(proc, &ei))
        return true;

    Elf64_Addr sym = elfimg_symlookup(symname, &ei, 0);
    if (sym == -1)
        fprintf(stderr, "symbol not found\n");
    else
        printf("%lx\n", sym);

    return elfimg_unmap(&ei);
}


int CMD(flookup, "looks up the address of a function inside the main binary",
        s_proc *proc, int argc, char *argv[])
{
    if (argc < 2)
    {
        fprintf(stderr, "missing function name\n");
        return CMD_FAILURE;
    }

    return CMD_FAILURE * proc_lookup_symbol(proc, argv[1]);
}
