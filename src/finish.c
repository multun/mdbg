#include "alloc.h"
#include "backtrace.h"
#include "breakpoint.h"
#include "commands.h"
#include "process.h"

#include <libunwind-ptrace.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>



static bool find_caller_sub(unw_cursor_t *cursor, unw_word_t *res)
{
    int step_status = unw_step(cursor);
    if (step_status < 0)
    {
        fprintf(stderr, "unw_step failed: %s\n", unw_strerror(step_status));
        return true;
    }

    if (!step_status)
    {
        fprintf(stderr, "there does not seem to be any caller to break in\n");
        return true;
    }

    if (!unw_get_reg(cursor, UNW_REG_IP, res))
        return false;

    fprintf(stderr, "cannot read the program counter\n");
    return true;
}


static bool find_caller(unw_addr_space_t as, pid_t pid, unw_word_t *res)
{
    void *context = _UPT_create(pid);
    if (!context)
    {
        fprintf(stderr, "failed to allocate the libunwind-ptrace context");
        return true;
    }

    bool status = true;
    unw_cursor_t cursor;
    if (unw_init_remote(&cursor, as, context) != 0)
        fprintf(stderr, "failed to initialize remote unwinding cursor\n");
    else
        status = find_caller_sub(&cursor, res);

    _UPT_destroy(context);
    return status;
}


int CMD(finish, "tbreak when the current function returns",
        s_proc *proc, int argc, char *argv[])
{
    unw_word_t caller_pc;
    if (find_caller(proc->unw_as, proc->pid, &caller_pc)
        || proc_add_breakpoint(proc, (void*)caller_pc, false))
        return CMD_FAILURE;

    return CMD_CONT;
}
