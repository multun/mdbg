#pragma once

#include "process.h"

#include <stdbool.h>
#include <sys/ptrace.h>


/**
** \brief continues a process, selecting the appropriate ptrace request
** \param proc the process to continue
** \return whether the operation failed
*/
bool proc_cont(s_proc *proc);


/**
** \brief calls the ptrace backend for proc, sending signal sig
** \param proc the process to operate
** \param request the ptrace request
** \param sig the signal to transmit
** \return whether the operation failed
*/
bool proc_sigtrace(s_proc *proc, enum __ptrace_request request, int sig);


/**
** \brief calls ptrace for process proc, updating its state if necessary
** \desc all the other parameters are the same as the original ptrace syscall
** \param proc the process to continue
** \return whether the operation failed
*/
bool proc_trace(s_proc *proc, enum __ptrace_request request,
                void *addr, void *data);


/**
** \brief reads a long from a process
** \param proc the process to peek at
** \param addr where to peek at
** \param res where to store the result
** \return whether the operation failed
*/
bool proc_peek(s_proc *proc, void *addr, long *res);


/**
** \brief writes a long to a process
** \param proc the process poke at
** \param addr where to write the data
** \param data the data to write
** \return whether the operation failed
*/
bool proc_poke(s_proc *proc, void *addr, long data);


/**
** \brief singlesteps into a process
** \param proc the process to step into
** \return whether the operation failed
*/
bool proc_singlestep(s_proc *proc);
