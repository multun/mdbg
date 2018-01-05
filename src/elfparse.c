#include "elfparse.h"
#include "macro_utils.h"
#include "proc_regs.h"

#include <elf.h>
#include <stdio.h>
#include <string.h>


/*
** There are quite a lot of sanity checks missing here
*/


static char *elfimg_strtab(s_elfimg *ei, int section)
{
    Elf64_Ehdr *ehdr = (void*)ei->data;
    Elf64_Off soff = ehdr->e_shoff;

    Elf64_Off str_soff = soff + (section * ehdr->e_shentsize);
    if (str_soff + ehdr->e_shentsize > ei->size)
    {
        fprintf(stderr, "out of bound string shdr table table");
        return NULL;
    }

    Elf64_Shdr *str_shdr = OFFSET(ei->data, str_soff);
    if (str_shdr->sh_offset + str_shdr->sh_size > ei->size)
    {
        fprintf(stderr, "out of bound string table table");
        return NULL;
    }

    return ei->data + str_shdr->sh_offset;
}


static Elf64_Shdr *elfimg_shdr(s_elfimg *ei)
{
    Elf64_Ehdr *ehdr = (void*)ei->data;
    Elf64_Off soff = ehdr->e_shoff;

    if (soff + ehdr->e_shnum * ehdr->e_shentsize > ei->size)
    {
        fprintf(stderr, "out of bound section table");
        return NULL;
    }

    return OFFSET(ei->data, soff);
}


static Elf64_Addr get_symaddr(Elf64_Sym *sym, Elf64_Addr load_offset)
{
    Elf64_Addr res = sym->st_value;
    if (sym->st_shndx != SHN_ABS)
        res += load_offset;
    return res;
}


// TODO: add more sanity checks
Elf64_Addr elfimg_symlookup(const char *symname, s_elfimg *ei,
                            Elf64_Addr load_offset)
{
    Elf64_Shdr *shdr = elfimg_shdr(ei);
    if (!shdr)
        return -1;

    Elf64_Ehdr *ehdr = (void*)ei->data;

    for (int i = 0; i < ehdr->e_shnum;
         i++, shdr = OFFSET(shdr, ehdr->e_shentsize))
    {
        if (shdr->sh_type != SHT_SYMTAB
            && shdr->sh_type != SHT_DYNSYM)
            continue;

        Elf64_Sym *sym = OFFSET(ei->data, shdr->sh_offset);
        Elf64_Sym *symtab_end = OFFSET(sym, shdr->sh_size);
        size_t sym_size = shdr->sh_entsize;

        char *strtab = elfimg_strtab(ei, shdr->sh_link);
        if (!strtab)
            continue;

        for (; sym < symtab_end; sym = OFFSET(sym, sym_size))
            if (ELF64_ST_TYPE (sym->st_info) == STT_FUNC
                && sym->st_shndx != SHN_UNDEF)
                // val may need to be altered depending
                // on the elf ABI version

                if (!strcmp(symname, strtab + sym->st_name))
                    return get_symaddr(sym, load_offset);
    }

    return -1;
}
