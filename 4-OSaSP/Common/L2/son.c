#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>

#define MAX_LENGTH 512

// message to print at process info header
const char* INFO_MESSAGE = 
    "Process data:"
    "\tName: %s\n"
    "\tPID: %d\n"
    "\tPPID: %d\n";

int main(int argc, char* argv[])
{
    if(argc < 2)
    {
        fprintf(stderr, "error: one argument expected but %d provided", argc - 1);
        exit(EXIT_FAILURE);
    }

    printf(INFO_MESSAGE, argv[0], getpid(), getppid());

    FILE* input = fopen(argv[1], "r");

    if(!input){
        perror("fopen");
        exit(errno);
    }

    char buffer[MAX_LENGTH];

    while(fgets(buffer, sizeof(buffer), input) != NULL) 
    {
        char* x = strchr(buffer, '\n');

        if(x){
            *x = '\0';
        }

        char* env = getenv(buffer);

        if(env){
            printf("%s=%s\n", buffer, env);
        }
        else{
            printf("warning: `%s` environment value doesn't exists\n", buffer);
        }
    }

    fclose(input);
    exit(EXIT_SUCCESS);
}