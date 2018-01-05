#pragma once


#include "mlist.h"
#include "process.h"

#include <stdbool.h>


typedef struct breakpoint
{
    s_mlist_e bplist;
    char orig_data;
    void *addr;
    bool persistant;
} s_breakpoint;


#define BPLIST(F) F(s_breakpoint, bplist)


#define BREAKPOINT(Addr, Persistant)      \
    (s_breakpoint)                              \
    {                                           \
        .addr = (Addr),                         \
        .persistant = (Persistant),             \
    }


/**
** \brief adds a breakpoint
** \param proc the process to add a breakpoint into
** \param addr the address to break at
** \return whether the operation succeeded
*/
bool proc_add_breakpoint(s_proc *proc, void *addr, bool persistant);


/**
** \brief prepares to step over a breakpoint, by decrementing EIP
** \param proc the process to prepare
*/
bool proc_breakpoint_prepare(s_proc *proc);


/**
** \brief enables a breakpoint by writting the appropriate
**   instruction to the process's memory
** \param proc the process to set the breakpoint into
** \param bp the breakpoint to set
*/
bool proc_breakpoint_enable(s_proc *proc, s_breakpoint *bp);


/**
** \brief disables a breakpoint by writting back the original data
**   to the process memory
** \param proc the process to set the breakpoint into
** \param bp the breakpoint to set
*/
bool proc_breakpoint_disable(s_proc *proc, s_breakpoint *bp);


/**
** \brief steps over a breakpoint
** \param proc the process to perform the operation in
*/
bool proc_breakpoint_step(s_proc *proc);


/**
** \brief disable all breakpoints
** \return whether the operation succeeded
*/
bool proc_breakpoint_disable_all(s_proc *proc);


/**
** \brief enable all breakpoints
** \return whether the operation succeeded
*/
bool proc_breakpoint_enable_all(s_proc *proc);
