#include "auxv.h"
#include "commands.h"
#include "elfparse.h"
#include "func_resolve.h"
#include "proc_bin_map.h"
#include "process.h"


static bool proc_get_base(s_proc *proc, s_elfimg *vei, Elf64_Addr *res)
{
    Elf64_Ehdr *ei = (void*)vei->data;
    Elf64_Addr bin_ep = ei->e_entry;

    unsigned long mem_ep;
    if (proc_auxv_get(proc, AT_ENTRY, &mem_ep))
        return true;

    *res = mem_ep - bin_ep;
    return false;
}


Elf64_Addr proc_lookup_symbol(s_proc *proc, const char *symname)
{
    s_elfimg ei;
    if (proc_map_bin(proc, &ei))
        return true;

    Elf64_Addr sym = -1;
    Elf64_Addr base;
    if (proc_get_base(proc, &ei, &base))
        goto exit;

    sym = elfimg_symlookup(symname, &ei, base);
    if (sym == -1)
    {
        fprintf(stderr, "symbol not found\n");
        goto exit;
    }

exit:
    return elfimg_unmap(&ei) ? -1 : sym;
}


int CMD(flookup, "looks up the address of a function inside the main binary",
        s_proc *proc, int argc, char *argv[])
{
    if (argc < 2)
    {
        fprintf(stderr, "missing function name\n");
        return CMD_FAILURE;
    }

    Elf64_Addr addr = proc_lookup_symbol(proc, argv[1]);
    if (addr == -1)
        return CMD_FAILURE;

    printf("0x%lx\n", addr);
    return CMD_SUCCESS;
}
