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
    int listCount = 1;
    char *pipe = strtok(cmd_line,PIPE_STRING);
    //clist->num = listCount;
    //strcpy(clist->commands->exe,"test1");
    //strcpy(clist->commands->args,"asd sad as");
    //printf("%d\n%s\n%s\n",clist->num,clist->commands->exe,clist->commands->args);


    while(pipe != NULL){
        printf("<%d> ",listCount);
        //printf("%s\n",pipe);
        char exeArr[EXE_MAX];
        int i = 0;
        int exeIndex = 0;
        int argIndex = 0;
        int endOfEXE = 0;
        int len = strlen(pipe);
        while(i < len){
            if(pipe[i] != ' '){
                exeArr[exeIndex] = pipe[i];
                exeIndex++;
                endOfEXE++;
            }
            else if(pipe[i] == ' ' && i >0){
                endOfEXE = i+1;
                break;
            }
            i++;
        }
        exeArr[exeIndex] = '\0';
        printf("%s",exeArr);
        //printf("\n%d %d\n",endOfEXE,len);
        if(endOfEXE < len-1){
            char argArr[ARG_MAX];
            while(endOfEXE < len-1){
                argArr[argIndex] = pipe[endOfEXE];
                endOfEXE++;
                argIndex++;
            }
            if(pipe[endOfEXE] != ' '){
                argArr[argIndex] = pipe[endOfEXE];
                endOfEXE++;
                argIndex++;
            }
            argArr[argIndex] = '\0';
            printf(" [%s]",argArr);
        }

        listCount++;
        pipe= strtok(NULL,PIPE_STRING);
        printf("\n");
    }
    return EXIT_NOT_IMPL;
}