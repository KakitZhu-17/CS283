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
    int cmdlineLen = strlen(cmd_line);
    int rc = 0;
    for(int i = 0; i < cmdlineLen; i++){
        if(cmd_line[i] == '|'){ //tracks hwo many pip chararter there are
            rc++;
        }
    }
    rc++;

    if(rc > CMD_MAX){
        return ERR_TOO_MANY_COMMANDS;
    }else{
        printf(CMD_OK_HEADER,rc);
    }


    int listCount = 1;
    char *pipe = strtok(cmd_line,PIPE_STRING); //splits the command line by the "|" 
    while(pipe != NULL){
        int i = 0;
        int len = strlen(pipe);
        int trueLen =len-1;
        while( pipe[trueLen] == ' '){ //this just decrements until we reach a charqacter that not white space(this is to remove excess white space at the end)
            trueLen--;
        }

        if(trueLen > 0){ //this just makes sure that the white space is not a command (this is for cases like "cmd arg1 | | cmd2 arg1" where a blank is between two '|' )
            printf("<%d> ",listCount);
            char exeArr[EXE_MAX];
            char argArr[ARG_MAX];
            int exeIndex = 0;
            int argIndex = 0;
            int endOfEXE = 0; //tracks the end of exe/command part of the string
            int endOfARG = 0;
            clist->num = listCount;
            while(i < len){//this is just for getting cmd by iteself
                if(pipe[i] != ' '){ //as long as the char isnt ' ' we will add it into the exe array, this also helps remove excess white space in the front of the substring
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
            if(endOfEXE > EXE_MAX){ //if command is too long
                return ERR_CMD_OR_ARGS_TOO_BIG;
            }
            strcpy(clist->commands->exe,exeArr); //adds/copies the exe array into the struct
            printf("%s ",clist->commands->exe);
           
            if(i < trueLen){
                i++;
                while(pipe[i] == ' '){ //removes white space in front of string
                    if(i > len){
                        break;
                    }
                    i++;
                }
                while(i < trueLen+1){
                    while(pipe[i] == ' ' && pipe[i+1] == ' '){ //removes any excess whitespace between arguments
                        i++;
                    }
                    argArr[argIndex] = pipe[i];
                    argIndex++;
                    endOfARG++;
                    i++;
                }
                argArr[argIndex] = '\0';
                if(endOfARG > ARG_MAX){
                    return ERR_CMD_OR_ARGS_TOO_BIG;
                }
                strcpy(clist->commands->args,argArr); //adds/copies the exe array into the struct
                printf("[%s]",clist->commands->args);
            }
            listCount++;
            printf("\n");
        }
        pipe= strtok(NULL,PIPE_STRING);
    }
    return OK;
}