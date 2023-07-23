#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <stdio.h>
#include <wait.h>

// comparator for sortenv(...)
static int sortenv_compar(const void* first, const void* second){
    return strcmp(*(const char**) first, *(const char**) second);
}

// sort process environment accordingly to LC_COLLATE=C
void sortenv(char** envp)
{
    char** e = envp;
    while(*e) e++;
    qsort(envp, e - envp, sizeof(char*), sortenv_compar);
}

// prints process environment
void printenv(char** envp)
{
    while(*envp){
        printf("%s\n", *envp++);
    }
}

// custom getenv() implementation with environ parameter
char* my_getenv(char** environ, const char* key)
{
    while(*environ)
    {
        if(strstr(*environ, key))
        {
            return *environ + strlen(key) + 1;
        }
        
        environ++;
    }
    
    return NULL;
}

int main(int argc, char* argv[], char* envp[])
{
    if(argc < 2)
    {
        fprintf(stderr, "error: one argument expected but %d provided", argc - 1);
        exit(EXIT_FAILURE);
    }

    sortenv(envp);
    printenv(envp);

    unsigned short counter = 0;

    while(counter < 100)
    {
        int option;
        char* path = NULL;

        switch(option = getchar())
        {
        case '+':
            path = getenv("CHILD_PATH");
            break;

        // `envp` is equivalent to `__environ`
        case '*':
            path = my_getenv(envp, "CHILD_PATH");
            break;

        case '&':
            path = my_getenv(__environ, "CHILD_PATH");
            break;

        case 'q':
            printf("info: parent process has been terminated using quit option\n");
            exit(EXIT_SUCCESS);

        default:
            if(option != '\n')
            {
                printf("warning: unknown option `%c` (%d)\n", option, (int) option);
            }

            continue;
        }

        if(path == NULL)
        {
            fprintf(stderr, "error: `CHILD_PATH` environment value doesn't exists\n");
            exit(EXIT_FAILURE);
        }

        char name[9];
        sprintf(name, "child_%02hu", counter++);

        pid_t pid = fork();

        if(pid > 0) // master process
        {
            int status;
            wait(&status);
        }
        else if(pid == 0) // fork process
        {
            char* const args[] = {name, argv[1], NULL};

            if(execve(path, args, envp) == -1)
            {
                perror("execve");
                exit(errno);
            }
        }
        else
        {
            perror("fork");
            exit(errno);
        }
    }

    printf("info: parent process has been terminated due to the `execve` counter limit being reached\n");
    exit(EXIT_SUCCESS);
}