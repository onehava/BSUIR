#define _GNU_SOURCE

#include "command.h"

#include <stdlib.h>
#include <string.h>

int pack_container(command_container_t* container, command_t* command)
{
    int size;
    int count;
    char* argument;

    if(command->argc > COMMAND_ARGUMENTS_SIZE){
        return EWRONGSTRUCT;
    }

    container->type = command->type;
    container->argc = command->argc;

    size = 0;

    for(int n = 0; n < command->argc; n++)
    {
        argument = *(command->argv + n);
        count = strlen(argument) + 1;

        if(size + count > COMMAND_CONTAINER_SIZE){
            return EBUFOVERFLOW;
        }

        memcpy(container->args + size, argument, count);
        size += count;
    }

    return 0;
}

#include <stdio.h>

int unpack_container(command_t* command, command_container_t* container)
{
    char* pointer;
    char* boundary;

    if(container->argc > COMMAND_ARGUMENTS_SIZE){
        return EWRONGSTRUCT;
    }

    command->type = container->type;
    command->argc = container->argc;

    pointer = container->args;
    boundary = container->args + COMMAND_CONTAINER_SIZE;

    for(int n = 0; n < container->argc; n++)
    {
        command->argv[n] = pointer;

        do {
            if(!(*pointer++)){
                break;
            }

            if(pointer > boundary){
                return EWRONGSTRUCT;
            }
        }
        while(1);
    }

    return 0;
}