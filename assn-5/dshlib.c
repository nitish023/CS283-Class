//Nitish Chawla, nkc47
//CS283, Assn5

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

 //main loop



 ///WORK///////
int exec_local_cmd_loop()
{
    char *cmd_buff = malloc(SH_CMD_MAX);
    command_list_t clist;

    while (1)
    {
        printf("%s", SH_PROMPT);
        if (fgets(cmd_buff, SH_CMD_MAX, stdin) == NULL)
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
        if (strcmp(cmd_buff, EXIT_CMD) == 0) 
        {
            break;
        }

        memset(&clist, 0, sizeof(command_list_t));
        int rc = build_cmd_list(cmd_buff, &clist);
        if (rc != OK) {
            if (rc == WARN_NO_CMDS) 
            {
                printf(CMD_WARN_NO_CMD);
            } else if (rc == ERR_TOO_MANY_COMMANDS) 
            {
                printf(CMD_ERR_PIPE_LIMIT, CMD_MAX);
            }
            continue;
        }

        execute_pipeline(&clist);
        free_cmd_list(&clist);
    }
    free(cmd_buff);
    return OK;
}

///////WORKS//////
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

///////WORKS////////
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
    char *cmd_copy = strdup(cmd_line);
    if (cmd_copy == NULL) 
    {
        return ERR_MEMORY;
    }
    token = strtok_r(cmd_line, PIPE_STRING, &saveptr);
    
    while (token != NULL)
    {
        if (total_cmds >= CMD_MAX)
        {
            free(cmd_copy);
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
            free(cmd_copy);
            return WARN_NO_CMDS;
        }

        char *cmd_string = strdup(token);
        if (cmd_string == NULL) 
        {
            free(cmd_copy);
            return ERR_MEMORY;
        }
        //main part that is different from prior assignment
        int rc = build_cmd_buff(cmd_string, &clist->commands[total_cmds]);
        if (rc != OK) 
        {
            free(cmd_copy);
            return rc;
        }

        total_cmds++;
        token = strtok_r(NULL, PIPE_STRING, &saveptr);
    }

    free(cmd_copy);
    clist->num = total_cmds;
    return OK;
}

///frees up everything
int free_cmd_list(command_list_t *cmd_lst) 
{
    if (cmd_lst == NULL) 
    {
        return ERR_MEMORY;
    }
    for (int i = 0; i < cmd_lst->num; i++) 
    {
        if (cmd_lst->commands[i]._cmd_buffer != NULL) 
        {
            free(cmd_lst->commands[i]._cmd_buffer);
            cmd_lst->commands[i]._cmd_buffer = NULL;
        }
    }
    cmd_lst->num = 0;
    return OK;
}

//for in-built commands as it is in the last assignment
////WORKS//////
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

///WORKS////
Built_In_Cmds match_command(const char *input) 
{
    if (input == NULL) 
    {
        return BI_NOT_BI;
    }
    if (strcmp(input, EXIT_CMD) == 0) 
    {
        return BI_CMD_EXIT;
    }
    if (strcmp(input, "cd") == 0) 
    {
        return BI_CMD_CD;
    }
    return BI_NOT_BI;
}

//in case of a single command
//WORKS/////
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

//in case of multiple commands
int execute_pipeline(command_list_t *clist)
{
    if (clist == NULL || clist->num == 0) 
    {
        return ERR_EXEC_CMD;
    }

    //this is if there is no pipeline i.e there is only one command
    int total_cmds = clist->num;
    if (total_cmds == 1) {
        Built_In_Cmds cmd_type = exec_built_in_cmd(&clist->commands[0]);
        if (cmd_type == BI_EXECUTED) {
            return OK;
        }
        return exec_cmd(&clist->commands[0]);
    }
    
    int pipe_fds[CMD_MAX-1][2];
    pid_t pids[CMD_MAX];

    for (int i = 0; i < total_cmds - 1; i++) {
        if (pipe(pipe_fds[i]) == -1) {
            for (int j = 0; j < i; j++) 
            {
                close(pipe_fds[j][0]);
                close(pipe_fds[j][1]);
            }
            return ERR_EXEC_CMD;
        }
    }
    
    for (int i = 0; i < total_cmds; i++) {
        if (i == 0) 
        {
            Built_In_Cmds cmd_type = exec_built_in_cmd(&clist->commands[i]);
            if (cmd_type == BI_EXECUTED) 
            {
                for (int j = 0; j < clist->num - 1; j++) 
                {
                    close(pipe_fds[j][0]);
                    close(pipe_fds[j][1]);
                }
                return OK;
            }
        }
        pids[i] = fork();
        
        if (pids[i] < 0) {
            for (int j = 0; j < total_cmds - 1; j++) 
            {
                close(pipe_fds[j][0]);
                close(pipe_fds[j][1]);
            }
            return ERR_EXEC_CMD;
        } 
        else if (pids[i] == 0) 
        {
            if (i > 0) 
            {
                dup2(pipe_fds[i-1][0], STDIN_FILENO);
            }
            if (i < total_cmds - 1) 
            {
                dup2(pipe_fds[i][1], STDOUT_FILENO);
            }
            for (int j = 0; j < total_cmds - 1; j++) 
            {
                close(pipe_fds[j][0]);
                close(pipe_fds[j][1]);
            }
        
            execvp(clist->commands[i].argv[0], clist->commands[i].argv);
            printf(CMD_ERR_EXECUTE);
            return ERR_EXEC_CMD;
        }
    }
    
    for (int i = 0; i < total_cmds - 1; i++) 
    {
        close(pipe_fds[i][0]);
        close(pipe_fds[i][1]);
    }

    int status;
    for (int i = 0; i < total_cmds; i++) 
    {
        waitpid(pids[i], &status, 0);
        if (i == total_cmds - 1) 
        {
            last_command_code = WEXITSTATUS(status);
        }
    }
    return OK;
}