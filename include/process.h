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
    int pid;
    e_proc_ev ev;
    int ev_data;
    bool coredump;
} s_proc;


#define PROC(Pid)          \
    (s_proc)               \
    {                      \
        .pid = (Pid),      \
        .ev_data = -1,     \
        .coredump = false, \
        .ev = PROC_INIT,   \
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
*/
void proc_update(s_proc *proc, int status);


/**
** \brief prints a description of the state of a process on stderr
** \param proc the process to describe
*/
void proc_describe(s_proc *proc);
