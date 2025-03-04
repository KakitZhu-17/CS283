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
    int trueLen =len;
    cmd_buff->_cmd_buffer = cmd_line;
    while( cmd_line[trueLen] == ' '){ //this just decrements until we reach a charqacter that not white space(this is to remove excess white space at the end)
        trueLen--;
    }

    if(trueLen > 0){ //this just makes sure that the white space is not a command (this is for cases like "cmd arg1 | | cmd2 arg1" where a blank is between two '|' )
        while(i < trueLen){//this is just for getting cmd by iteself
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
            else if(cmd_line[i] != ' ' && i < trueLen){ //as long as the char isnt ' ' we will add it into the exe array, this also helps remove excess white space in the front of the substring
                char *cmdArr= malloc(sizeof(char)*ARG_MAX);
                int cmdIndex = 0;
                while(cmd_line[i] != ' ' && i < trueLen){
                    cmdArr[cmdIndex] = cmd_line[i];
                    i++;
                    cmdIndex++;
                    if(i > trueLen || cmd_line[i+1] == ' '){
                        break;
                    }
                }
                cmdArr[cmdIndex] = cmd_line[i];
                cmdIndex++;
                cmdArr[cmdIndex] = '\0';
                cmd_buff->argv[argCounter]=cmdArr;
                argCounter++;
                    
            }
            i++;
        }
    }

    cmd_buff->argc= argCounter;
    return argCounter;
}


int execute_pipeline(command_list_t *clist){
    int rc = 0;
    int pipeArr[clist->num - 1][2]; //array of file descriptors for the pipes
    pid_t pids[clist->num];

    for(int i = 0; i < clist->num; i++) {
        int createPipes =pipe(pipeArr[i]);
        if (createPipes == -1) {
            perror("");
            exit(-1);
        }
    }
    
    int currentCmd = 0;
    while(currentCmd < clist->num+1){
        pids[currentCmd] = fork();
    
        if(pids[currentCmd] == 0) {  
               
            if (currentCmd != 0) { //this will only work if its not the first command
                int dupOne = dup2(pipeArr[currentCmd-1][0], STDIN_FILENO); //gets output previous command (the read end) 
                if(dupOne == -1){
                    perror("");
                    exit(-1);
                }
            }

            if (currentCmd < clist->num) {
                int dupTwo =dup2(pipeArr[currentCmd][1], STDOUT_FILENO); //gets/duplicates current process output into the write end
                if(dupTwo == -1){
                    perror("");
                    exit(-1);
                }
            }

            for (int j = 0; j < clist->num; j++) { //closes all pipes of the child process
                close(pipeArr[j][0]); //closes read 
                close(pipeArr[j][1]); //closes write 
            }
            int exe = execvp(clist->commands[currentCmd].argv[0], clist->commands[currentCmd].argv);
            if(exe == -1){
                perror("");
                exit(errno);
            }
        }
        else if(pids[currentCmd] == -1){
            perror("");
            exit(-1);

        }
        currentCmd++;
    }

    for (int i = 0; i < clist->num; i++) { //same idea but for the parent process
        close(pipeArr[i][0]);
        close(pipeArr[i][1]);
    }
    int exitStat;
    for (int i = 0; i < clist->num+1; i++) {
        int waitStat = waitpid(pids[i], &exitStat, 0);
        if(waitStat == -1){
            perror("");
            exit(-1);
        }
        int currentExitStat = WEXITSTATUS(exitStat); 
        if(currentExitStat != 0){
            rc = currentExitStat;
        }
    }
    return rc;
}


int exec_local_cmd_loop()
{
    char *cmd_buff= malloc(sizeof(char)*SH_CMD_MAX);
    int rcFirstState = 0; //this will be used to see if rc is the first command or not, without this it, it would return a random number if no commands used beforehand

    while(1){
        int rc;
        if(rcFirstState == 0){
            rc = 0;
        }
        int cmdCount = 0;
        printf("%s", SH_PROMPT);
        if (fgets(cmd_buff, ARG_MAX, stdin) == NULL){
            printf("\n");
            break;
        }
        //remove the trailing \n from cmd_buff
        cmd_buff[strcspn(cmd_buff,"\n")] = '\0';
        int len = strlen(cmd_buff);
        for(int i = 0; i < len;i++){ //counts pipe chars
            if(cmd_buff[i] == '|'){
                cmdCount++;
            }
        }
        if(cmdCount > CMD_MAX-1){
            printf(CMD_ERR_PIPE_LIMIT,CMD_MAX);
        }
        
        else if(strcmp(cmd_buff,"exit") == 0){ //the exit commmand
            break;
        }
        else if(len == 0){
            printf(CMD_WARN_NO_CMD);
        }
        else{
            int currentCmdindex = 0;
            command_list_t commandList = {0};
            commandList.num = cmdCount;
            char *pipe = strtok(cmd_buff,PIPE_STRING);
            while(pipe != NULL){
                cmd_buff_t cmd = {0};
                build_cmd_buff(pipe,&cmd);
                commandList.commands[currentCmdindex] = cmd;
                currentCmdindex++;
                pipe= strtok(NULL,PIPE_STRING);
            }

            if(strcmp(commandList.commands[0].argv[0],"cd") == 0){ 
                if(commandList.commands[0].argc >= 2){
                    int changeDir = chdir(commandList.commands[0].argv[1]);
                    if(changeDir == -1){
                        perror("cd");
                        rc = 1; //chdir returns -1 so i will just return 1 to be safe
                        rcFirstState = 1;
                    }
                }
            }
            else if(strcmp(commandList.commands[0].argv[0],"rc") == 0){  //rc command
                printf("%d\n",rc);
                rc =0;
                rcFirstState = 1;
            }
            else{    
                int exePipeline = execute_pipeline(&commandList);
                rc =exePipeline;
                rcFirstState = 1;

            }

            for(int a = 0;a < commandList.num+1;a++){ 
                for(int b = 0; b < commandList.commands[a].argc;b++){
                    free(commandList.commands[a].argv[b]);
                }
            }
            
        }

    }
    free(cmd_buff);
    return OK;
}
