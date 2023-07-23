#define _GNU_SOURCE

#include "session.h"

#include <errno.h>

#include <dirent.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>

int init_state(session_state_t* state)
{
    if(!getcwd(state->path, sizeof(state->path))){
        state->runs = 0;
        return -1;
    }
    else {
        state->runs = 1;
        return 0;
    }
}

static int concat_path(char* dest, size_t size, const char* src1, const char* src2)
{
    size_t size1 = strlen(src1);
    size_t size2 = strlen(src2);

    if(size1 + size2 + 1 > size){
        errno = EOVERFLOW;
        return -1;
    }

    dest[0] = 0;
    strcat(dest, src1);
    strcat(dest, "/");
    strcat(dest, src2);

    return 0;
}

int set_directory(session_state_t* state, const char* path)
{
    char root[PATH_MAX];
    char temp[PATH_MAX];
    char next[PATH_MAX];
    
    struct stat statistics;

    if(!getcwd(root, sizeof(root))){
        return -1;
    }

    if(*path == '/')
    {
        if(concat_path(temp, sizeof(temp), root, path)){
            return -1;
        }
    }
    else 
    {
        if(concat_path(temp, sizeof(temp), state->path, path)){
            return -1;
        }
    }

    if(stat(temp, &statistics)){
        return -1;
    }

    if(!S_ISDIR(statistics.st_mode)){
        return -1;
    }

    // info: this is a vulnerability, because user can explore file tree under cwd using error response
    if(!realpath(temp, next)){
        return -1;
    }

    if(strncmp(next, root, strlen(root)))
    {
        strcpy(state->path, root);
    }
    else {
        strcpy(state->path, next);
    }

    return 0;
}

int get_directory_entries(session_state_t* state, struct dirent*** entries)
{
    return scandir(state->path, entries, 0, alphasort);
}