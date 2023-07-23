#pragma once

typedef struct {
    int memsize;
    int sections;
    int threads;
    const char* name;
}
command;

command parse(int argc, char** argv);