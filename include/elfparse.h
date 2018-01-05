#pragma once

#include "ebuf.h"

#include <elf.h>


typedef s_ebuf s_elfimg;


Elf64_Addr elfimg_symlookup(const char *symname, s_elfimg *ei,
                            Elf64_Addr load_offset);
