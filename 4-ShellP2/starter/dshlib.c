#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include "dshlib.h"
#include <errno.h>

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
 *      CMD_ERR_EXECUTE        print on execution failure of external command   //note to grader: this is not in the header file so I just used perrer() for any error messages
 * 
 *  Standard Library Functions You Might Want To Consider Using (assignment 1+)
 *      malloc(), free(), strlen(), fgets(), strcspn(), printf()
 * 
 *  Standard Library Functions You Might Want To Consider Using (assignment 2+)
 *      fork(), execvp(), exit(), chdir()
 */
int build_cmd_buff(char *cmd_line, cmd_buff_t *cmd_buff){
    int argCounter = 0;
    char *pipe = strtok(cmd_line,PIPE_STRING); //splits the command line by the "|" 
    while(pipe != NULL){
        int i = 0;
        int len = strlen(pipe);
        int trueLen =len-1;
        while( pipe[trueLen] == ' '){ //this just decrements until we reach a charqacter that not white space(this is to remove excess white space at the end)
            trueLen--;
        }

        if(trueLen > 0){ //this just makes sure that the white space is not a command (this is for cases like "cmd arg1 | | cmd2 arg1" where a blank is between two '|' )
            while(i < trueLen+1){//this is just for getting cmd by iteself
                if(pipe[i] == '"'){ //this deals with quotes (it iterates through until it sees another " char)
                    char *cmdArr = malloc(sizeof(char)*ARG_MAX);
                    int cmdIndex = 0;
                    i++;
                    while(pipe[i] != '"' && i < trueLen){ 
                        cmdArr[cmdIndex] = pipe[i];
                        cmdIndex++;
                        i++;
                    }
                    if(pipe[i] == '"'){
                        cmdArr[cmdIndex] = '\0';
                        cmd_buff->argv[argCounter]=cmdArr;
                    }
                    argCounter++;
                }
                else if(pipe[i] != ' ' && i < trueLen+1){ //as long as the char isnt ' ' we will add it into the exe array, this also helps remove excess white space in the front of the substring
                    char *cmdArr= malloc(sizeof(char)*ARG_MAX);
                    int cmdIndex = 0;
                    while(pipe[i] != ' ' && i < trueLen+1){
                        cmdArr[cmdIndex] = pipe[i];
                        i++;
                        cmdIndex++;
                        if(i > trueLen+1 || pipe[i+1] == ' '){
                            break;
                        }
                    }
                    cmdArr[cmdIndex] = pipe[i];
                    cmdIndex++;
                    cmdArr[cmdIndex] = '\0';
                    //printf("outer %s|\n",cmdArr);
                    cmd_buff->argv[argCounter]=cmdArr;
                    argCounter++;
                    
                }
                i++;
            }
        }
        pipe= strtok(NULL,PIPE_STRING);
    }
    cmd_buff->argc= argCounter;
    char cmdBuffer[strlen(cmd_line)];
    int cmdBufferIndex = 0;
    
    for(int i =0;i<argCounter;i++){
        int argvLen = strlen(cmd_buff->argv[i]);
        for(int j = 0; j < argvLen;j++){
            cmdBuffer[cmdBufferIndex] = cmd_buff->argv[i][j];
            cmdBufferIndex++;
            if(i != argCounter && j == argvLen){
                cmdBuffer[cmdBufferIndex] = ' ';

            }
        }
    }
    cmdBuffer[cmdBufferIndex] = '\0';
    cmd_buff->_cmd_buffer = cmdBuffer;
    return argCounter;


}


int exec_local_cmd_loop()
{
    char *cmd_buff= malloc(sizeof(char)*SH_CMD_MAX);
    cmd_buff_t cmd = {0};
    

    // TODO IMPLEMENT MAIN LOOP

    // TODO IMPLEMENT parsing input to cmd_buff_t *cmd_buff

    // TODO IMPLEMENT if built-in command, execute builtin logic for exit, cd (extra credit: dragon)
    // the cd command should chdir to the provided directory; if no directory is provided, do nothing

    // TODO IMPLEMENT if not built-in command, fork/exec as an external command
    // for example, if the user input is "ls -l", you would fork/exec the command "ls" with the arg "-l"

    while(1){
        int rc;
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

            int argCounter =build_cmd_buff(cmd_buff,&cmd);

            if(strcmp(cmd.argv[0],"cd") == 0){ 
                if(cmd.argc >= 2){
                    int changeDir = chdir(cmd.argv[1]);
                    if(changeDir == -1){
                        perror("cd");
                        rc = changeDir;
                    }
                    else{
                        rc =0;
                    }
                }
            }
            else if(strcmp(cmd.argv[0],"rc") == 0){  //rc command
                printf("%d\n",rc);
                rc = 0;
                
            }
            else if(strcmp(cmd.argv[0],"cd") != 0){
                int childResult;
                int childProcess = fork();
                int error;
                if(childProcess == -1){
                    perror("");
                }
                if(childProcess == 0){
                    int exe = execvp(cmd.argv[0],cmd.argv);
                    if (exe == -1){
                        perror(""); 
                        error =errno;
                        exit(error);
                        
                    }
                }
                else{
                    int waitStat = wait(&childResult);
                    int extractExitCode = WEXITSTATUS(childResult);
                    rc = extractExitCode;
                    if(waitStat == -1){
                        perror("");
                        rc = waitStat;
                    }
                }

            }

            for(int i = 0;i < argCounter;i++){ //frees memory and resets it for next use
                free(cmd.argv[i]);
                cmd.argv[i] = NULL;
            }
            
            
        }

    }
    free(cmd_buff);
    return OK;
}
