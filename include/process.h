#pragma once

#include <stdbool.h>


typedef enum proc_ev
{
    PROC_INIT,      // the process startup is pending
    PROC_EXITED,    // the process is dead
    PROC_SIGNALED,  // killed by signal
    PROC_STOPPED,   // stopped by signal, proc.signal contains the signal id
    PROC_TRAPPED,   // received a trap
    PROC_SYSCALL,   // inside a syscall
    PROC_CONTINUED, // continued
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
} s_proc;


#define PROC(Pid)            \
    (s_proc)                 \
    {                        \
        .setup_done = false, \
        .pid = (Pid),        \
        .exit_status = -1,   \
        .signal = 0,         \
        .coredump = false,   \
        .ev = PROC_INIT,     \
    }


/**
** \brief is the process still alive?
** \param proc the process to test
*/
bool proc_alive(s_proc *proc);


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
