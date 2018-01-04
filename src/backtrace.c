#include "alloc.h"
#include "backtrace.h"
#include "commands.h"
#include "process.h"

#include <libunwind-ptrace.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>


bool backtrace_init(s_proc *proc)
{
    proc->unw_as = unw_create_addr_space(&_UPT_accessors, 0);
    if (proc->unw_as)
        return false;

    fprintf(stderr, "failed to initialize libunwind\n");
    return true;
}


static bool backtrace_format(unw_cursor_t *cursor, unw_word_t pc,
                             char *symname_buf, FILE *of)
{
    fprintf(of, "<0x%lx> ", pc);

    unw_word_t off;
    int status = unw_get_proc_name(cursor, symname_buf, SYMNAME_MAX_SIZE, &off);
    if (status < 0)
    {
        fprintf(stderr, "unw_get_proc_name failed: %s\n",
                unw_strerror(status));
        return true;
    }

    if (!status)
        fprintf(of, "(%s+0x%lx)\n", symname_buf, off);
    else
        fprintf(of, "????\n");

    return false;
}


static bool backtrace_iter(unw_cursor_t *cursor, char *symname_buf, FILE *of)
{
    int step_status;
    do {
        unw_word_t pc;
        if (unw_get_reg(cursor, UNW_REG_IP, &pc))
        {
            fprintf(stderr, "cannot read the program counter\n");
            return true;
        }

        if (backtrace_format(cursor, pc, symname_buf, of))
            return true;

        step_status = unw_step(cursor);
        if (step_status < 0)
        {
            fprintf(stderr, "unw_step failed: %s\n", unw_strerror(step_status));
            return true;
        }
    } while (step_status);

    return false;
}


static bool backtrace(unw_addr_space_t as, pid_t pid, FILE *of)
{
    void *context = _UPT_create(pid);
    if (!context)
    {
        fprintf(stderr, "failed to allocate the libunwind-ptrace context");
        return true;
    }

    char *symname_buf = xmalloc(SYMNAME_MAX_SIZE);

    bool status = true;
    unw_cursor_t cursor;
    if (unw_init_remote(&cursor, as, context) != 0)
        fprintf(stderr, "failed to initialize remote unwinding cursor\n");
    else
        status = backtrace_iter(&cursor, symname_buf, of);

    _UPT_destroy(context);
    free(symname_buf);
    return status;
}


void proc_backtrace(s_proc *proc)
{
    backtrace(proc->unw_as, proc->pid, stdout);
}


int CMD(backtrace, "prints a backtrace",
        s_proc *proc, int argc, char *argv[])
{
    proc_backtrace(proc);
    return CMD_SUCCESS;
}
