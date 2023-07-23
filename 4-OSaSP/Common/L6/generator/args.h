#pragma once

typedef struct {
    int size;
    const char* name;
}
command;

command parse(int args, char** argv);