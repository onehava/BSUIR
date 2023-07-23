#define _GNU_SOURCE

#include "prompt.h"
#include "command.h"
#include "message.h"

#include <stdio.h>
#include <errno.h>

#include <stdlib.h>
#include <string.h>
#include <wctype.h>
#include <unistd.h>

#include <stdio.h>

// Character stream interaction

static ssize_t readln(FILE* stream, char** dest)
{
    size_t n;
    return getline(dest, &n, stream);
}

static ssize_t find_next_mean(const char* src, ssize_t pos)
{
    while(iswspace(src[pos]))
    {
        if(src[pos] == '\n'){
            return -1;
        }

        pos++;
    }

    return pos;
}

static ssize_t find_next_zero(const char* src, ssize_t pos)
{
    while(!iswspace(src[pos])){
        pos++;
    }

    return pos;
}

// Prepare command for processing

static int get_command_type(const char* word)
{
    static const char* const names[5] = {"info", "echo", "list", "cd", "quit"};

    for(int i = 0; i < 5; i++){
        if(!strcmp(word, names[i])){
            return i;
        }
    }

    return -1;
}

static int prepare_command_sequence(char* src, ssize_t pos, command_t* cmd)
{
    int count = 0;
    char* words[COMMAND_ARGUMENTS_SIZE + 1];

    while(1)
    {
        if(count >= COMMAND_ARGUMENTS_SIZE + 1){
            return -1;
        }

        ssize_t start = pos = find_next_mean(src, pos);

        if(pos == -1){
            break;
        }

        pos = find_next_zero(src, pos);

        if(src[pos] == '\n')
        {
            src[pos] = '\0';
            words[count++] = src + start;
            break;
        }
        else {
            src[pos++] = '\0';
            words[count++] = src + start;
            continue;
        }
    }

    if(count){
        cmd->type = get_command_type(words[0]);
        cmd->argc = count - 1;

        for(int n = 1; n < count; n++)
        {
            cmd->argv[n - 1] = words[n];
        }
    }
    else {
        cmd->type = -1;
        cmd->argc = -1;
    }

    return 0;
}

// Run prompt

static void run_prompt(FILE* stream, int server_sock, int nestence)
{
    char* input;
    ssize_t count;
    ssize_t position;

    input = NULL;
    count = readln(stream, &input);

    if(count <= 0){
        goto exit;
    }

    position = find_next_mean(input, 0);

    if(position == -1){
        goto exit;
    }

    if(input[position] == '@')
    {
        const char* name = input + position + 1;

        position = find_next_zero(input, position);
        input[position] = 0;

        FILE* script = fopen(name, "r");

        if(script == NULL){
            fprintf(stderr, "prompt2: %s\n", strerror(errno));
            goto exit;
        }

        run_script_prompt(script, server_sock, nestence + 1);
        fclose(script);
    }
    else {
        int status;
        command_t command;
        command_container_t container;
        message_t response;

        status = prepare_command_sequence(input, position, &command);

        if(status == -1){
            fprintf(stdout, "prompt: error parsing prompt (too many arguments)\n");
            goto exit;
        }

        if(command.type == -1){
            fprintf(stdout, "prompt: command not found\n");
            goto exit;
        }

        status = pack_container(&container, &command);

        if(status){
            fprintf(stderr, "internal error: wrong command structure\n");
            goto exit;
        }

        status = write(server_sock, &container, sizeof(container));

        if(status == -1){
            fprintf(stderr, "internal error: %s\n", strerror(errno));
            goto exit;
        }

        status = read(server_sock, response.data, MESSAGE_MAX);

        if(status == -1){
            fprintf(stderr, "internal error: %s\n", strerror(errno));
            goto exit;
        }

        if(status == 0)
        {
            close(server_sock);
            free(input);
            exit(EXIT_SUCCESS); // todo: fix me
        }

        response.data[status] = 0; // todo: fix me

        printf("%s\n", response.data);
    }

exit:
    free(input);
}

void run_script_prompt(FILE* stream, int server_sock, int nestence)
{
    if(nestence > PROMPT_NESTENCE_MAX)
    {   
        fprintf(stdout, "prompt: recursive execution has limited nestence\n");
        return;
    }

    while(!feof(stream))
    {
        run_prompt(stream, server_sock, nestence);
    }
}

void run_interactive_prompt(int server_sock)
{
    while(!feof(stdin))
    {
        printf("> ");
        run_prompt(stdin, server_sock, 0);
    }
}