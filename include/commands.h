#pragma once

#include "process.h"

#include <stdio.h>


typedef int (*f_cmd)(s_proc *proc, int argc, char *argv[]);


typedef struct cmd
{
    // the name of the command
    // it must not contain spaces
    const char *name;

    // the function to run when the command is called
    f_cmd func;

    // the documentation string of the command
    // it will be displayed by the help command
    const char *doc;

    // apparently LD aligns data on a 16b basis when merging sections
} __attribute__((aligned(16))) s_cmd;


#define CMD_SEC cmds
#define SEC_FORMAT(Pos, Name) __ ## Pos ## _ ## Name
#define APPLY2(F, Arg1, Arg2) F(Arg1, Arg2)
#define CMD_SEC_START APPLY2(SEC_FORMAT, start, CMD_SEC)
#define CMD_SEC_STOP APPLY2(SEC_FORMAT, stop, CMD_SEC)


#define STR(Arg) #Arg
#define CMD_ATTR(Sec)                           \
    __attribute__((used, section(STR(Sec))))


extern const s_cmd CMD_SEC_START[];
extern const s_cmd CMD_SEC_STOP[];


/**
** \brief iterate over all available commands
** \param IName the desired name of the iterator variable
*/
#define CMD_FOREACH(IName)                              \
    for (const s_cmd *IName = CMD_SEC_START;            \
         IName < CMD_SEC_STOP; IName++)


#define CMD_FNAME(Name) __cmd_func__ ## Name


/**
** \brief declare a new command
** \desc use as follows:
**   int CMD(help, "displays help", s_proc *proc, int argc, char *argv)
**   {
**   }
** \param Name the name of the command to declare, as a valid C identifier
** \param Doc a string describing the usage of the command
*/
#define CMD(Name, Doc, ...)                                     \
    static CMD_FNAME(Name) (__VA_ARGS__);                       \
    static CMD_ATTR(CMD_SEC) const s_cmd __cmd_ ## Name =       \
    {                                                           \
        .name = #Name,                                          \
        .func = CMD_FNAME(Name),                                \
        .doc = (Doc),                                           \
    };                                                          \
    static int CMD_FNAME(Name) (__VA_ARGS__)


/**
** \brief runs a command
** \param proc the process to apply the command on
** \param argc the argument count
** \param argv tge argument array
** \return the command's status code
*/
int cmd_run(s_proc *proc, int argc, char *argv[]);


/**
** \brief parses and runs a command
** \param proc the process to apply the command on
** \param cmdline the untouched user input line
** \return the command's status code
*/
int cmd_parserun(s_proc *proc, char *cmdline);


// does the command require the process to continue?
#define CMD_CONT 2
#define CMD_SUCCESS 0
#define CMD_FAILURE 1
#define CMD_EXIT 4
#define CMD_FATAL (CMD_FAILURE | CMD_EXIT)
#define CMD_NOT_FOUND 8

#define CMD_SUCCEEDED(Code) ((Code) & 1)
