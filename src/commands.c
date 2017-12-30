#include "commands.h"
#include "mvect.h"

#include <string.h>


static const s_cmd *cmd_get(const char *name)
{
    CMD_FOREACH(cur)
        if (!strcmp(cur->name, name))
            return cur;

    return NULL;
}


int cmd_run(s_proc *proc, int argc, char *argv[])
{
    if (!argv[0])
        return CMD_SUCCESS;

    const s_cmd *cmd = cmd_get(argv[0]);
    if (!cmd)
        return CMD_NOT_FOUND | CMD_FAILURE;

    return cmd->func(proc, argc, argv);
}


int cmd_parserun(s_proc *proc, char *cmdline)
{
    if (!cmdline)
        return CMD_EXIT | CMD_NOT_FOUND;

    s_mvect vec;
    MVECT_INIT(&vec, char*, 5);
    const char delim[] = " \t";
    int argc = 0;
    for (char *blk = strtok(cmdline, delim); blk; blk = strtok(NULL, delim))
        if (*blk)
        {
            MVECT_PUSH(&vec, char*, blk);
            argc++;
        }

    MVECT_PUSH(&vec, char*, NULL);
    return cmd_run(proc, argc, MVECT_DATA(&vec, char*));
}


int CMD(help, "displays this help",
        s_proc *proc, int argc, char *argv[])
{
    CMD_FOREACH(cur)
        if (cur->doc)
            fprintf(stderr, "%s\t%s\n", cur->name, cur->doc);

    return CMD_SUCCESS;
}
