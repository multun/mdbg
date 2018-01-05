#pragma once

#include "elfparse.h"
#include "process.h"

#include <stdbool.h>


/**
** \brief map a process's executable into memory
** \param proc the process to map the executable file of
** \param res the elfimg to store the result in
** \return whether the operation failed
*/
bool proc_map_bin(s_proc *proc, s_elfimg *res);


/**
** \brief unmap a process's executable from memory
** \param img the image to unmap
** \return whether the operation failed
*/
bool elfimg_unmap(s_elfimg *img);
