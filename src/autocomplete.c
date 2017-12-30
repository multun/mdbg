#include "commands.h"

#include <stdlib.h>
#include <string.h>

#include <stdio.h>
#include <readline/readline.h>


static char *cmd_generator(const char *text, int state)
{
    static size_t len;
    static const s_cmd *cur;

    if (!state)
    {
        cur = CMD_SEC_START;
        len = strlen(text);
    }

    while (cur++ < CMD_SEC_STOP)
        if (strncmp(cur[-1].name, text, len) == 0)
            return strdup(cur[-1].name);

    return NULL;
}


static char **cmd_completion(const char *text, int start, int end)
{
    rl_attempted_completion_over = 1;
    return rl_completion_matches(text, cmd_generator);
}


void autocomplete_setup(void)
{
    rl_attempted_completion_function = cmd_completion;
}
