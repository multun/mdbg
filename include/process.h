#pragma once

#include "mvect.h"

#include <stdbool.h>


typedef enum proc_ev
{
    PROC_INIT,      // the process startup is pending
    PROC_STOPPED,   // stopped by signal, proc.signal contains the signal id
    PROC_BREAKPOINT,// hit a software breakpoint
    PROC_TRAPPED,   // received a trap from the kernel
    PROC_SINGLESTEP,// single-stepping
    PROC_SYSCALL,   // inside a syscall
    PROC_CONTINUED, // continued
    PROC_EXITED,    // the process is dead
    PROC_SIGNALED,  // killed by signal
} e_proc_ev;


typedef struct proc
{
    bool setup_done;
    int pid;
    e_proc_ev ev;
    int exit_status;
    int signal;
    bool deliver_signal;
    bool coredump;
    s_mvect watched_syscalls;
} s_proc;


#define PROC(Pid)                           \
    (s_proc)                                \
    {                                       \
        .setup_done = false,                \
        .pid = (Pid),                       \
        .exit_status = -1,                  \
        .signal = 0,                        \
        .coredump = false,                  \
        .ev = PROC_INIT,                    \
        .watched_syscalls = MVECT_DEFAULT,  \
    }


/**
** \brief is the process still alive?
** \param Proc the process to test
*/
#define PROC_ALIVE(Proc) ((Proc)->ev < PROC_EXITED)


/**
** \brief updates the internal process state metadata
** \param proc the process to update
** \param status the status as returned by waitpid
** \return whether the operation failed
*/
bool proc_update(s_proc *proc, int status);


/**
** \brief prints a description of the state of a process on stderr
** \param proc the process to describe
*/
void proc_describe(s_proc *proc);


/**
** \brief wait for a process to change state
** \desc doing so also updates the internal s_proc metadata
** \param proc the process to wait for
** \return whether the operation failed
*/
bool proc_wait(s_proc *proc);


/**
** \param proc the process to examine
** \return whether the current process state requires user attention
*/
bool proc_is_userstop(s_proc *proc);
