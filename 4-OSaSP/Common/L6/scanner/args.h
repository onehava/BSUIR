#pragma once

typedef struct {
    const char* name;
}
command;

command parse(int argc, char** argv);