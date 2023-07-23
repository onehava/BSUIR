#pragma once

#include <dirent.h>

typedef struct {
    int runs;
    char path[PATH_MAX];
}
session_state_t;

int init_state(session_state_t* state);

int set_directory(session_state_t* state, const char* path);

int get_directory_entries(session_state_t* state, struct dirent*** entries);