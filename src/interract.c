#include "commands.h"
#include "interract.h"

#include <stdio.h>
#include <stdlib.h>

#include <readline/readline.h>
#include <readline/history.h>



int interract(s_proc *proc)
{
    int status = 0;
    do {
        char *line = readline("> ");
        add_history(line);
        pcmd((status = cmd_parserun(proc, line)));
        free(line);
    } while (!(status & (CMD_CONT | CMD_EXIT)));
    return status;
}
