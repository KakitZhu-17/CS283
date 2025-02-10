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
    char *pipe = strtok(cmd_line,PIPE_STRING); //splits the command line by the "|" 
    while(pipe != NULL){
        int i = 0;
        int len = strlen(pipe);
        int trueLen =len-1;
        printf("\nstr len %d , i = %d\n",trueLen , i);
        while( pipe[trueLen] == ' '){
            trueLen--;
        }
        printf("<%d> ",listCount);
        char exeArr[EXE_MAX];
        char argArr[ARG_MAX];
        int exeIndex = 0;
        int argIndex = 0;
        int endOfEXE = 0; //tracks the end of exe/command part of the string
        clist->num = listCount;
        while(i < len){
            if(pipe[i] != ' '){ //as long as the char isnt ' ' we will add it into the exe array 
                exeArr[exeIndex] = pipe[i];
                exeIndex++;
                endOfEXE++;
                if(pipe[i + 1] == ' '){
                    break;
                }
            }
            i++;
        }
        exeArr[exeIndex] = '\0';
        //strcpy(clist->commands->exe,exeArr); //adds/copies the exe array into the struct
        printf("%s ",exeArr);
        //printf("\nstr len %d , i = %d\n",trueLen , i);
        if(i < trueLen){
            i++;
            while(pipe[i] == ' '){ //removes white space in front of string
                if(i > len){
                    break;
                }
                i++;
            }
            while(i < trueLen+1){
                while(pipe[i] == ' ' && pipe[i+1] == ' '){
                    i++;
                }
                argArr[argIndex] = pipe[i];
                argIndex++;
                i++;
            }
            argArr[argIndex] = '\0';
            printf("[%s]",argArr);
        }

        listCount++;
        pipe= strtok(NULL,PIPE_STRING);
        printf("\n");
    }
    return EXIT_NOT_IMPL;
}