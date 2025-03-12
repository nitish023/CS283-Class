//Nitish Chawla, nkc47
//CS283, Assn3

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "dshlib.h"

/*
 *  build_cmd_list
 *    cmd_line:     the command line from the user
 *    clist *:      pointer to clist structure to be populated
 *
 *  This function builds the command_list_t structure passed by the caller
 *  It does this by first splitting the cmd_line into commands by spltting
 *  the string based on any pipe characters '|'.  It then traverses each
 *  command.  For each command (a substring of cmd_line), it then parses
 *  that command by taking the first token as the executable name, and
 *  then the remaining tokens as the arguments.
 *
 *  NOTE your implementation should be able to handle properly removing
 *  leading and trailing spaces!
 *
 *  errors returned:
 *
 *    OK:                      No Error
 *    ERR_TOO_MANY_COMMANDS:   There is a limit of CMD_MAX (see dshlib.h)
 *                             commands.
 *    ERR_CMD_OR_ARGS_TOO_BIG: One of the commands provided by the user
 *                             was larger than allowed, either the
 *                             executable name, or the arg string.
 *
 *  Standard Library Functions You Might Want To Consider Using
 *      memset(), strcmp(), strcpy(), strtok(), strlen(), strchr()
 */
int build_cmd_list(char *cmd_line, command_list_t *clist)
{
    //making sure again
    if (cmd_line == NULL || clist == NULL)
    {
        return WARN_NO_CMDS;
    }

    memset(clist, 0, sizeof(command_list_t));
    int total_cmds = 0;
    char *token; //each part of the cmd_line
    char *saveptr;
    token = strtok_r(cmd_line, PIPE_STRING, &saveptr);
    
    while (token != NULL)
    {
        if (total_cmds >= CMD_MAX)
        {
            return ERR_TOO_MANY_COMMANDS;
        }
        ;
        //checking spaces
        while (*token == SPACE_CHAR)
        {
            token++;
        }
        char *end = token+strlen(token)-1;
        while (end > token && *end == SPACE_CHAR)
        {
            *end = '\0';
            end--;
        }
        if (strlen(token) == 0)
        {
            return WARN_NO_CMDS;
        }

        char *arg_begin = strchr(token, SPACE_CHAR);
        if (arg_begin)
        {
            *arg_begin = '\0';
            arg_begin++;
        
            while (*arg_begin == SPACE_CHAR)
            {
                arg_begin++;
            }
        }

        if (strlen(token) >= EXE_MAX || (arg_begin && strlen(arg_begin) >= ARG_MAX))
        {
            return ERR_CMD_OR_ARGS_TOO_BIG;
        }

        strncpy(clist->commands[total_cmds].exe, token, EXE_MAX-1);
        if (arg_begin)
        {
            strncpy(clist->commands[total_cmds].args, arg_begin, ARG_MAX-1);
        }

        total_cmds++;
        token = strtok_r(NULL, PIPE_STRING, &saveptr);
    }

    clist->num = total_cmds;
    return OK;
}