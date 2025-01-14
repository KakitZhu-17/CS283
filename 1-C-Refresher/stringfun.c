#include <stdio.h>
#include <string.h>
#include <stdlib.h>


#define BUFFER_SZ 50

//prototypes
void usage(char *);
void print_buff(char *, int);
int  setup_buff(char *, char *, int);

//prototypes for functions to handle required functionality
int  count_words(char *, int, int);
//add additional prototypes here


int setup_buff(char *buff, char *user_str, int len){
    //TODO: #4:  Implement the setup buff as per the directions
    int wordLen = 0;
    int lenTracker = 0; //we will use this to track the length and make sure there is a correct ammount of '.' along with checking if the input string is bigger than BUFFER_SZ
    while(*user_str != '\0'){
        if(*user_str == ' '){ //this detects if the character is a white space
            while(*(user_str+1) == ' '){ //this detects if there is a duplicate white space and iterates through the string until it reaches a non whitespace 
                user_str++;
            }
        }
        //printf("%c",*user_str);
        *buff = *user_str; //this adds/copies the user string character into the buffer
        wordLen++; //increments up the wordLen variable
        user_str++; //incremnets up by one to move onto the next char
        buff++; //increments up by 1 so we can assign chars into the next buffer index
        lenTracker++;
        if(lenTracker > len){
            return -1;
        }
    }
    while(lenTracker < len){ //this while loop basically adds '.' until lentracker is the same length as BUFFER_SZ
        *buff = '.'; 
        buff++;
        lenTracker++;
    }
    //printf("char count %d\n", wordLen);
    return wordLen; //for now just so the code compiles. 
}

void print_buff(char *buff, int len){
    printf("Buffer:  ");
    for (int i=0; i<len; i++){
        putchar(*(buff+i));
    }
    putchar('\n');
}

void usage(char *exename){
    printf("usage: %s [-h|c|r|w|x] \"string\" [other args]\n", exename);

}

int count_words(char *buff, int len, int str_len){
    int i = 0;
    int WordTracker = 0;
    //printf("strlen %d\n",str_len);
    while(i < len){
        //printf("%c",*buff);
        if(*buff == ' ' && i < str_len){
            WordTracker++;
        }
        buff++;
        i++;
    }
    WordTracker++;
    //printf("\n");
    return WordTracker;
}

void strReversed(char *buff, int str_len){
    int i = str_len;
    char *lastChar= buff + str_len-1; //this just gets the last character of our buffer
    while(i != 0){
        printf("%c", *lastChar);
        lastChar--; //we increment down by 1 to print out the reversed string
        i--; 
    }
    printf("\n");
}

void wordPrint(char* buff, int len,  int str_len){
    printf("Word Print\n");
    printf("----------\n");
    int wc = count_words(buff,len,str_len);
    int i = 0;
    int j = 0;
    int charCount = 0;
    while(i < wc){
        printf("%d. ",i+1);
        int wordLen = 0;
        while((*buff != ' ') && j < str_len){
            printf("%c",*buff);
            wordLen++;
            buff++;
            j++;
            charCount++;
        }
        printf(" (%d)",wordLen);
        buff++;
        j++;
        printf("\n");
        i++;
    }
}

//ADD OTHER HELPER FUNCTIONS HERE FOR OTHER REQUIRED PROGRAM OPTIONS

int main(int argc, char *argv[]){

    char *buff;             //placehoder for the internal buffer
    char *input_string;     //holds the string provided by the user on cmd line
    char opt;               //used to capture user option from cmd line
    int  rc;                //used for return codes
    int  user_str_len;      //length of user supplied string

    //TODO:  #1. WHY IS THIS SAFE, aka what if arv[1] does not exist?
    //      PLACE A COMMENT BLOCK HERE EXPLAINING
    // Answer: argv[1] is for getting the command line argument 
    // in our case in this assignment it will be for getting the string we want to modify
    // without it argv we cannot get the string we want to modify
    if ((argc < 2) || (*argv[1] != '-')){
        usage(argv[0]);
        exit(1);
    }

    opt = (char)*(argv[1]+1);   //get the option flag

    //handle the help flag and then exit normally
    if (opt == 'h'){
        usage(argv[0]);
        exit(0);
    }

    //WE NOW WILL HANDLE THE REQUIRED OPERATIONS

    //TODO:  #2 Document the purpose of the if statement below
    //      PLACE A COMMENT BLOCK HERE EXPLAINING
    // Answer: this code is checking if the argument count is less than 3. if it is less than three 
    // then it will call the functions usage which prints out a message
    if (argc < 3){
        usage(argv[0]);
        exit(1);
    }

    input_string = argv[2]; //capture the user input string

    //TODO:  #3 Allocate space for the buffer using malloc and
    //          handle error if malloc fails by exiting with a 
    //          return code of 99
    // CODE GOES HERE FOR #3
    buff = malloc(BUFFER_SZ);
    if(buff == NULL){
        printf("failed to allocate memory"); //this message will print if malloc fails
        exit(99); //this will give a return code of 99
    }

    user_str_len = setup_buff(buff, input_string, BUFFER_SZ);     //see todos
    if (user_str_len < 0){
        printf("Error setting up buffer, error = %d", user_str_len);
        exit(2);
    }

    switch (opt){
        case 'c':
            rc = count_words(buff, BUFFER_SZ, user_str_len);  //you need to implement
            if (rc < 0){
                printf("Error counting words, rc = %d", rc);
                exit(2);
            }
            printf("Word Count: %d\n", rc);
            break;
        //TODO:  #5 Implement the other cases for 'r' and 'w' by extending
        //       the case statement options
        case 'r':
            strReversed(buff,user_str_len);
            break;

        case 'w':
            wordPrint(buff,BUFFER_SZ,user_str_len);
            break;
        default:
            usage(argv[0]);
            exit(1);
    }

    //TODO:  #6 Dont forget to free your buffer before exiting
    print_buff(buff,BUFFER_SZ);
    exit(0);
}

//TODO:  #7  Notice all of the helper functions provided in the 
//          starter take both the buffer as well as the length.  Why
//          do you think providing both the pointer and the length
//          is a good practice, after all we know from main() that 
//          the buff variable will have exactly 50 bytes?
//  
//          PLACE YOUR ANSWER HERE