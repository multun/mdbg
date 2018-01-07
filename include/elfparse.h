#pragma once

#include "ebuf.h"

#include <elf.h>


/**
** \brief an elf image loaded into memory
*/
typedef s_ebuf s_elfimg;


/**
** \brief looks up a symbol into an elf image
** \param symname the name of the symbol to lookup
** \param ei the elf image to lookup into
** \param load_offset the offset at which the image is loaded
**   in the remote process
** \return the address of the symbol, or -1 if none is found
*/
Elf64_Addr elfimg_symlookup(const char *symname, s_elfimg *ei,
                            Elf64_Addr load_offset);
