//Nitish Chawla, nkc47
//CS283, Assn4

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <errno.h>
#include "dshlib.h"
int last_command_code = 0;

/*
 * Implement your exec_local_cmd_loop function by building a loop that prompts the 
 * user for input.  Use the SH_PROMPT constant from dshlib.h and then
 * use fgets to accept user input.
 * 
 *      while(1){
 *        printf("%s", SH_PROMPT);
 *        if (fgets(cmd_buff, ARG_MAX, stdin) == NULL){
 *           printf("\n");
 *           break;
 *        }
 *        //remove the trailing \n from cmd_buff
 *        cmd_buff[strcspn(cmd_buff,"\n")] = '\0';
 * 
 *        //IMPLEMENT THE REST OF THE REQUIREMENTS
 *      }
 * 
 *   Also, use the constants in the dshlib.h in this code.  
 *      SH_CMD_MAX              maximum buffer size for user input
 *      EXIT_CMD                constant that terminates the dsh program
 *      SH_PROMPT               the shell prompt
 *      OK                      the command was parsed properly
 *      WARN_NO_CMDS            the user command was empty
 *      ERR_TOO_MANY_COMMANDS   too many pipes used
 *      ERR_MEMORY              dynamic memory management failure
 * 
 *   errors returned
 *      OK                     No error
 *      ERR_MEMORY             Dynamic memory management failure
 *      WARN_NO_CMDS           No commands parsed
 *      ERR_TOO_MANY_COMMANDS  too many pipes used
 *   
 *   console messages
 *      CMD_WARN_NO_CMD        print on WARN_NO_CMDS
 *      CMD_ERR_PIPE_LIMIT     print on ERR_TOO_MANY_COMMANDS
 *      CMD_ERR_EXECUTE        print on execution failure of external command
 * 
 *  Standard Library Functions You Might Want To Consider Using (assignment 1+)
 *      malloc(), free(), strlen(), fgets(), strcspn(), printf()
 * 
 *  Standard Library Functions You Might Want To Consider Using (assignment 2+)
 *      fork(), execvp(), exit(), chdir()
 */
int exec_local_cmd_loop()
{
    char cmd_buff[ARG_MAX];
    cmd_buff_t cmd;

    // TODO IMPLEMENT MAIN LOOP
    while (1)
    {
        printf("%s", SH_PROMPT); //Prompt
        if (fgets(cmd_buff, ARG_MAX, stdin) == NULL)
        {
            printf("\n");
            break;
        }
        cmd_buff[strcspn(cmd_buff, "\n")] = '\0';
        if (strlen(cmd_buff) == 0)
        {
            printf(CMD_WARN_NO_CMD);
            continue;
        }

        memset(&cmd, 0, sizeof(cmd_buff_t));
        if (build_cmd_buff(cmd_buff, &cmd) !=OK)
        {
            continue;
        }

        Built_In_Cmds cmd_type = exec_built_in_cmd(&cmd);
        if (cmd_type == BI_EXECUTED)
        {
            continue;
        }

        exec_cmd(&cmd);
    }
    return OK;
}

    // TODO IMPLEMENT parsing input to cmd_buff_t *cmd_buff
int build_cmd_buff(char *cmd_line, cmd_buff_t *cmd_buff)
{
    if (cmd_line == NULL || cmd_buff == NULL)
    {
        return WARN_NO_CMDS;
    }

    memset(cmd_buff, 0, sizeof(cmd_buff_t));

    cmd_buff->_cmd_buffer = strdup(cmd_line);
    if (cmd_buff->_cmd_buffer == NULL)
    {
        return ERR_MEMORY;
    }

    int arg_begin = 0;
    bool has_quotes = false;

    for (char *p = cmd_buff->_cmd_buffer; *p != '\0'; p++)
    {
        if (*p == '"')
        {
            has_quotes = !has_quotes;
            *p = '\0';
            continue;
        }
        else if (!has_quotes && *p == SPACE_CHAR)
        {
            *p = '\0';
            continue;
        }

        if (arg_begin == 0 || *(p-1) == '\0')
        {
            cmd_buff->argv[arg_begin++] = p;
        }
        if (arg_begin >= CMD_ARGV_MAX - 1)
        {
            free(cmd_buff->_cmd_buffer);
            return ERR_CMD_OR_ARGS_TOO_BIG;
        }
    }
    
    cmd_buff->argv[arg_begin] = NULL;
    cmd_buff->argc = arg_begin;

    return OK;
}


    // TODO IMPLEMENT if built-in command, execute builtin logic for exit, cd (extra credit: dragon)
    // the cd command should chdir to the provided directory; if no directory is provided, do nothing
Built_In_Cmds exec_built_in_cmd(cmd_buff_t *cmd)
{
    if (cmd->argc == 0)
    {
        return BI_NOT_BI;
    }
    if (strcmp(cmd->argv[0], EXIT_CMD) == 0)
    {
        exit(OK);
    }

    if (strcmp(cmd->argv[0], "cd") == 0)
    {
        if (cmd->argc == 1)
        {
            return BI_EXECUTED;
        }

        if (chdir(cmd->argv[1]) != 0)
        {
            printf(CMD_ERR_EXECUTE);
        }

        return BI_EXECUTED;
    }

    if (strcmp(cmd->argv[0], "rc") == 0)
    {
        printf("%d\n", last_command_code);
        return BI_EXECUTED;
    }

    return BI_NOT_BI;
}


    // TODO IMPLEMENT if not built-in command, fork/exec as an external command
    // for example, if the user input is "ls -l", you would fork/exec the command "ls" with the arg "-l"
int exec_cmd(cmd_buff_t *cmd)
{
    pid_t pid = fork();
    int status;

    if (pid < 0)
    {
        return ERR_EXEC_CMD;
    }
    if (pid == 0)
    {
        execvp(cmd->argv[0], cmd->argv);
        switch (errno)
        {
            case ENOENT:
                fprintf(stderr, "Command not found in PATH\n");
                exit(2);
            case EACCES:
                fprintf(stderr, "Permission denied\n");
                exit(3);
            default:
                fprintf(stderr, "Execution failed: %s\n", strerror(errno));
                exit(errno);
        }
    } else {
        waitpid(pid, &status, 0);
        last_command_code = WEXITSTATUS(status);
        return last_command_code;
    }
}
