//Nitish Chawla, nkc47
//Assignment 1, CS283

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
    char *des = buff;
    char *src = user_str;
    int length = 0;
    int space_flag = 0;

    while (*src != '\0'){
        if (length >= len) return -1;

        if (*src == ' ' || *src == '\t') 
        {
            if (!space_flag) {
                *des++ = ' ';
                length++;
                space_flag = 1; 
            }
        } else 
        {
            *des++ = *src;
            length++;
            space_flag = 0;
        }
        src++;
    }

    while (length < len)
    {
        *des++ = '.';
        length++;
    }

    return src - user_str;

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
    int count = 0;
    int i = 0;

    for (int k = 0; k < str_len; k++)
    {
        if (buff[k] != ' ')
        {
            if (!i)
            {
                count++;
                i = 1;
            }
        } else {
            i = 0;
        }

    }

    return count;
}

//ADD OTHER HELPER FUNCTIONS HERE FOR OTHER REQUIRED PROGRAM OPTIONS
void reverse_string(char *buff, int len, int str_len) {
    char *start = buff;
    char *end = buff + str_len - 1;

    while (end >= buff && *end == '.') {
        end--;
    }

    while (start < end) 
    {
        char temp = *start;
        *start = *end;
        *end = temp;
        start++;
        end--;
    }

    printf("Reversed String: %.*s\n", str_len, buff);
}

void replace_string(char *buff, int len, int str_len, char *target, char *replace) {
    char temp[len];
    char *pos = strstr(buff, target);
    int target_len = strlen(target);
    int replace_len = strlen(replace);

    int pre_target_len = pos - buff;

    if (pre_target_len + replace_len + (str_len - (pre_target_len + target_len)) > len) {
        printf("Buffer overflow\n");
        return;
    }

    strncpy(temp, buff, pre_target_len);
    temp[pre_target_len] = '\0';

    strncat(temp, replace, replace_len);
    strncat(temp, pos + target_len, str_len - (pre_target_len + target_len));

    strncpy(buff, temp, len-1);
    buff[len-1] = '\0';

    printf("Modified String: %s\n", buff);

}



void word_print(char *buff, int len, int str_len) {
    printf("Word Print\n----------\n");

    char *start = NULL;
    int count = 0;

    for (int i = 0; i < str_len; i++) {
        if (buff[i] != ' ' && buff[i] != '.') {
            if (start == NULL) {
                start = &buff[i];
                count++;
            }
        } else if (start != NULL) {
            int word_len = &buff[i] - start;
            printf("%d. %.*s (%d)\n", count, word_len, start, word_len);
            start = NULL; 
        }
    }

    if (start != NULL) {
        int word_len = &buff[str_len] - start;
        printf("%d. %.*s (%d)\n", count, word_len, start, word_len);
    }
}

int main(int argc, char *argv[]){

    char *buff;             //placehoder for the internal buffer
    char *input_string;     //holds the string provided by the user on cmd line
    char opt;               //used to capture user option from cmd line
    int  rc;                //used for return codes
    int  user_str_len;      //length of user supplied string

    //TODO:  #1. WHY IS THIS SAFE, aka what if arv[1] does not exist?
    //      Because the if condition ensures that the program exits if there are fewer than 2 arguments.
    //      The first argument is the program name, so argv[1] exists only if there are more than 1 argument.
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
    //      The if statement checks if the program has received the three required arguments. The name, the flag, and the string.
    //      If the program has not received the three required arguments, the program will print the usage and exit with a return code of 1.
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
    if (buff == NULL){
        printf("Error: Memory allocation failed\n");
        exit(99);
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
            reverse_string(buff, BUFFER_SZ, user_str_len);
            break;

        case 'w':
            word_print(buff, BUFFER_SZ, user_str_len);
            break;

        case 'x':
            if (argc < 5)
            {
                printf("Error: Missing target\n");
                usage(argv[0]);
                exit(1);
            }
            replace_string(buff, BUFFER_SZ, user_str_len, argv[3], argv[4]);
            break;

        default:
            usage(argv[0]);
            exit(1);
    }

    //TODO:  #6 Dont forget to free your buffer before exiting
    print_buff(buff,BUFFER_SZ);
    free(buff);
    exit(0);
}

//TODO:  #7  Notice all of the helper functions provided in the 
//          starter take both the buffer as well as the length.  Why
//          do you think providing both the pointer and the length
//          is a good practice, after all we know from main() that 
//          the buff variable will have exactly 50 bytes?
//  
//          PLACE YOUR ANSWER HERE
//          Providing both the pointer and the length is a good practice because it allows the functions to be more flexible.
//          This makes it easier if the buffer size is changed in the future. It also makes the functions more reusable if the program is redesigned.
