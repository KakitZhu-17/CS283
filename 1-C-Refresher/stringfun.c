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
    int strLenTracker = 0;
    int lenTracker = 0; //we will use this to track the length and make sure there is a correct ammount of '.' along with checking if the input string is bigger than BUFFER_SZ
    while(*user_str == ' ' && *user_str != '\0' ){//if there white space at the very beginning of the user inputted string then this will iterate forward until there is none
        user_str++;
    }
    while(*user_str != '\0'){
        if(*user_str == ' '){ //this detects if the character is a white space
            while(*(user_str+1) == ' '){ //this detects if there is a duplicate white space and iterates through the string until it reaches a non whitespace 
                user_str++;
            }
        }
        //printf("%c",*user_str);
        *buff = *user_str; //this adds/copies the user string character into the buffer
        strLenTracker++; //increments up the strLenTracker variable
        user_str++; //incremnets up by one to move onto the next char
        buff++; //increments up by 1 so we can assign chars into the next buffer index
        lenTracker++;
        if(lenTracker > len){
            return -1;
        }
    }
    while(*(buff-1)== ' '){ //if theres any white space at the end then this will remove it 
        buff--;
        lenTracker--;
    }
    //printf("len tracker %d\n", lenTracker);
    while(lenTracker < len){ //this while loop basically adds '.' until lentracker is the same length as BUFFER_SZ
        *buff = '.'; 
        buff++;
        lenTracker++;
    }

    return strLenTracker; //for now just so the code compiles. 
}

void print_buff(char *buff, int len){
    printf("Buffer:  ");
    printf("[");
    for (int i=0; i<len; i++){
        putchar(*(buff+i));
    }
    printf("]");
    putchar('\n');
}

void usage(char *exename){
    printf("usage: %s [-h|c|r|w|x] \"string\" [other args]\n", exename);

}

int count_words(char *buff, int len, int str_len){
    int i = 0;
    int WordTracker = 0;
    while(i < len){
        //printf("%c",*buff);
        if((*buff == ' ' || *buff == '.') && i < str_len-1){
            //printf("%c",*buff);
            WordTracker++;
        }
        buff++;
        i++;
    }
    WordTracker++;
    //printf("\n");
    return WordTracker;
}

void strReversed(char *buff,int len,int str_len){
    char *strBegin = buff;
    char *strEnd = buff + (str_len -1);
    int begintracker = 0;
    int endtracker = str_len;
    char temp;
    while(begintracker != endtracker){
        temp = *strBegin;
        *strBegin = *strEnd;
        *strEnd = temp;
        strBegin++;
        strEnd--;
        begintracker++;
        endtracker--;
    }
    //printf("beginT %d  endT%d",begintracker,endtracker);
    //printf("Reversed String: ");
    int i = 0;
    while(i < len){
        //printf("%c",*buff);
        buff++;
        i++;
    }
    //printf("\n");
}

void wordPrint(char* buff, int len,  int str_len){
    printf("Word Print\n");
    printf("----------\n");
    int wc = count_words(buff,len,str_len);
    int i = 0; //this is for keeping track of the word count
    int j = 0; //this keeps track of where we are in the buffer
    int charCount = 0; 
    while(i < wc){
        printf("%d. ",i+1); //prints the numbers
        int wordLen = 0; //this is for tracking the length of the individual words
        while((*buff != ' ') && j < str_len){
            printf("%c",*buff);
            wordLen++;
            buff++;
            j++;
            charCount++;
        }
        printf("(%d)",wordLen);
        buff++;
        j++;
        printf("\n");
        i++;
    }
    printf("\n");
    printf("Number of words returned: %d\n",wc);
}

int replace(char* buff, int str_len, char* targetWord, char* replacement ){
    //printf("Modified Word: ");
    int currentLen = 0;
    int targetWordStart;
    int targetWordEnd = 0;
    char *tempBuff = buff;
    char *afterReplacement = buff + (str_len-1);
    char *afterReplacement2 = buff;
    char *finalBuff = buff;
    int matchWordLen = 0;
    int firstOccurance = 0;
    while(currentLen <= 50){
        char *target = targetWord;
        int match = 0;
        while(*tempBuff == *target && firstOccurance == 0){
            if((*(tempBuff+1) == ' '|| *(tempBuff+1) == '.') && *(target+1) == '\0'){
                matchWordLen = match;
                targetWordEnd = currentLen;
                targetWordStart = targetWordEnd - match;
                firstOccurance = 1;
            }
            //printf("%c",*tempBuff);
            tempBuff++;
            target++;
            currentLen++;
            match++;
        }
        //printf("%c",*tempBuff);
        tempBuff++;
        currentLen++;
    }
    //printf("\n%d\n",matchWordLen);
    if(matchWordLen == 0){ //exits and gives -1 if the target word is not found
        //printf("\ncheck2\n");
        exit(-1);
        //return -1;
    }
    //printf("\ncheck\n");
    int i = 0;
    int replaceWordLen = 0;
    char temp;
    while(i < targetWordStart){
        //printf("%c",*buff);
        buff++;
        i++;
    }
    while(*replacement != '\0'){
        replacement++;
        replaceWordLen++;
    }
    int difference = replaceWordLen - (matchWordLen+1);
    int replacementWordEnd = targetWordEnd + difference;
    int shifting = (str_len + difference) - replacementWordEnd; 
    int secondLen = 0;
    //printf("difference %d\n",difference);
    //printf("target %d\n",targetWordEnd);
    //printf("replacementWordEnd:%d\n",replacementWordEnd);
    //printf("shifting %d\n",shifting);
    if(difference >= 0){
        while(secondLen < shifting){
            //printf("%c",*afterReplacement);
            temp = *afterReplacement;
            *(afterReplacement + difference) = temp;
            afterReplacement--;
            secondLen++;
        }
        int k = 0;
        while(k < replaceWordLen){
            replacement--;
            k++;
        }
        while(*replacement != '\0'){
            *buff = *replacement;
            replacement++;
            buff++;
        }
    }else if(difference < 0){
        int shiftingPoint = replacementWordEnd - difference;
        afterReplacement2+=shiftingPoint;
        int k = 0;
        while(k < replaceWordLen){
            replacement--;
            k++;
        }
        while(secondLen < shifting+(matchWordLen+1)){
            temp = *afterReplacement2;
            *(afterReplacement2 + difference) = temp;
            afterReplacement2++;
            secondLen++;
        }   
        while(*replacement != '\0'){
            *buff = *replacement;
            replacement++;
            buff++;
        }
    }
    return 0;
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
            if(user_str_len <= BUFFER_SZ){
                strReversed(buff,BUFFER_SZ,user_str_len);
            }
            else{
                exit(1);
            }
            break;

        case 'w':
            if(user_str_len <= BUFFER_SZ){
                wordPrint(buff,BUFFER_SZ,user_str_len);
            }
            else{
                exit(1);
            }
            break;

        case 'x':
            char *targetWord = argv[3];
            char *swapTo = argv[4];
            if(argc == 5){
                replace(buff,user_str_len,targetWord,swapTo);
            }
            else{
                exit(-1);
            }
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
//   Answer: its good to have both pointer and buffer size because like our assignment here we can track
//           where we are on the buffer and where it ends since the buffer here has no terminating character / '\0'.