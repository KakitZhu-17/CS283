#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>

#include "dshlib.h"

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


int build_cmd_buff(char *cmd_line, cmd_buff_t *cmd_buff){
    int argCounter = 0;
    int i = 0;
    int len = strlen(cmd_line);
    int trueLen =len-1;
    while( cmd_line[trueLen] == ' '){ //this just decrements until we reach a charqacter that not white space(this is to remove excess white space at the end)
        trueLen--;
    }

    if(trueLen > 0){ //this just makes sure that the white space is not a command (this is for cases like "cmd arg1 | | cmd2 arg1" where a blank is between two '|' )
        while(i < trueLen+1){//this is just for getting cmd by iteself
            if(cmd_line[i] == '"'){ //this deals with quotes (it iterates through until it sees another " char)
                char *cmdArr = malloc(sizeof(char)*ARG_MAX);
                int cmdIndex = 0;
                i++;
                while(cmd_line[i] != '"' && i < trueLen){ 
                    cmdArr[cmdIndex] = cmd_line[i];
                    cmdIndex++;
                    i++;
                }
                if(cmd_line[i] == '"'){
                    cmdArr[cmdIndex] = '\0';
                    cmd_buff->argv[argCounter]=cmdArr;
                }
                argCounter++;
            }
            else if(cmd_line[i] != ' ' && i < trueLen+1){ //as long as the char isnt ' ' we will add it into the exe array, this also helps remove excess white space in the front of the substring
                char *cmdArr= malloc(sizeof(char)*ARG_MAX);
                int cmdIndex = 0;
                while(cmd_line[i] != ' ' && i < trueLen+1){
                    cmdArr[cmdIndex] = cmd_line[i];
                    i++;
                    cmdIndex++;
                    if(i > trueLen+1 || cmd_line[i+1] == ' '){
                        break;
                    }
                }
                cmdArr[cmdIndex] = cmd_line[i];
                cmdIndex++;
                cmdArr[cmdIndex] = '\0';
                //printf("outer %s|\n",cmdArr);
                cmd_buff->argv[argCounter]=cmdArr;
                argCounter++;
                    
            }
                i++;
        }
    }

    cmd_buff->argc= argCounter;
    return argCounter;
}

int exec_local_cmd_loop()
{
    char *cmd_buff= malloc(sizeof(char)*SH_CMD_MAX);

    while(1){
        //cmd_buff_t cmd = {0};
        //int rc;
        printf("%s", SH_PROMPT);
        if (fgets(cmd_buff, ARG_MAX, stdin) == NULL){
            printf("\n");
            break;
        }
        //remove the trailing \n from cmd_buff
        cmd_buff[strcspn(cmd_buff,"\n")] = '\0';
        int len = strlen(cmd_buff);

        //IMPLEMENT THE REST OF THE REQUIREMENTS
        if(strcmp(cmd_buff,"exit") == 0){ //the exit commmand
            break;
        }
        if(len == 0){
            printf(CMD_WARN_NO_CMD);
        }
        else{

            char *pipe = strtok(cmd_buff,PIPE_STRING);
            while(pipe != NULL){
                printf("cmd: ");
                cmd_buff_t cmd = {0};
                int argCounter = build_cmd_buff(pipe,&cmd);
                for(int i = 0;i < argCounter;i++){ //frees memory and resets it for next use
                    printf("%s ", cmd.argv[i]);
                }
                printf("\n");
                //printf("cmd %s \n", pipe);
                pipe= strtok(NULL,PIPE_STRING);
            }

            //int argCounter =build_cmd_buff(cmd_buff,&cmd);
            //for(int i = 0;i < argCounter;i++){ //frees memory and resets it for next use
            //    printf("%s ", cmd.argv[i]);
            //}
            //printf("|\n");

            //if(strcmp(cmd.argv[0],"cd") == 0){ 
            //    if(cmd.argc >= 2){
            //        int changeDir = chdir(cmd.argv[1]);
            //        if(changeDir == -1){
            //            perror("cd");
            //            rc = changeDir;
            //        }
            //    }
            //}
            //if(strcmp(cmd.argv[0],"rc") == 0){  //rc command
            //    printf("%d\n",rc);
            //    rc = 0;
            //    
            //}
            
            //for(int i = 0;i < argCounter;i++){ //frees memory and resets it for next use
            //    free(cmd.argv[i]);
            //    cmd.argv[i] = NULL;
            //}        
            
        }

    }
    free(cmd_buff);
    return OK;
}
