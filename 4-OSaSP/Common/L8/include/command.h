#pragma once

#define COMMAND_CONTAINER_SIZE 1024
#define COMMAND_ARGUMENTS_SIZE 10

enum command_type {
    INFO,
    ECHO,
    LIST,
    CD,
    QUIT,
};

typedef struct {
    int type;
    int argc;
    char* argv[COMMAND_ARGUMENTS_SIZE];
}
command_t;

typedef struct {
    int type;
    int argc;
    char args[COMMAND_CONTAINER_SIZE];
}
command_container_t;

enum command_container_error 
{
    EBUFOVERFLOW,
    EWRONGSTRUCT,
};

int pack_container(command_container_t* container, command_t* command);

int unpack_container(command_t* command, command_container_t* container);