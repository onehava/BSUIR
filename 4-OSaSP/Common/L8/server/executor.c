#define _GNU_SOURCE

#include "executor.h"
#include "unimsg.h"

#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void execute(command_t* cmd, session_state_t* state, message_t* response)
{
    switch(cmd->type)
    {
        case INFO:
        {
            if(cmd->argc){
                msgset(response, "info: too many arguments");
                return;
            }
            
            msgset(response, "very informative message");
            return;
        }
        
        case ECHO:
        {
            switch(cmd->argc)
            {
            case 0:
                msgset(response, "echo: missing argument");
                return;

            case 1:
                msgset(response, cmd->argv[0]);
                return;

            default:
                msgset(response, "echo: too many arguments");
                return;
            }
        }

        case LIST:
        {
            int count;
            struct dirent** entries;

            int shift;
            char root[PATH_MAX];

            if(cmd->argc){
                msgset(response, "list: too many arguments");
                return;
            }

            if(!getcwd(root, sizeof(root))){
                msgset(response, "internal error: can't get current working directory");
                return;
            }

            shift = strlen(root);

            count = get_directory_entries(state, &entries);

            if(count == -1){
                msgset(response, "internal error: can't get current directory entries");
                return;
            }

            for(int i = 0; i < count; i++)
            {
                struct dirent* entry = entries[i];

                if(!strcmp(entry->d_name, ".") || !strcmp(entry->d_name, "..")){
                    continue;
                }

                msgcat(response, entry->d_name);

                if(entry->d_type == DT_LNK)
                {
                    char path[PATH_MAX];

                    if(realpath(entry->d_name, path)){
                        msgcat(response, " -> ");
                        
                        if(strncmp(path, root, shift)){
                            msgcat(response, "*");
                        }
                        else {
                            msgcat(response, "~");
                            msgcat(response, path + shift);
                        }
                    }
                }

                free(entry);
                
                if(i < count - 1){
                    msgcat(response, "\n");
                }
            }

            if(!response->size){
                *response->data = 0;
                response->size = 1;
            }

            free(entries);
            return;
        }

        case CD:
        {
            int status;

            switch(cmd->argc)
            {
            case 0:
                set_directory(state, "/");
                break;

            case 1:
                status = set_directory(state, cmd->argv[0]);

                if(status){
                    msgset(response, "cd: no such directory");
                    return;
                }

                break;

            default:
                msgset(response, "cd: too many arguments");
                return;
            }

            char path[PATH_MAX];
            int shift;

            if(!getcwd(path, sizeof(path))){
                msgset(response, "internal error: can't get current directory entries");
                return;
            }

            shift = strlen(path);

            msgset(response, "~");
            msgcat(response, state->path + shift);
            return;
        }

        case QUIT:
        {
            if(cmd->argc){
                msgset(response, "quit: too many arguments");
                return;
            }
            
            state->runs = 0;
            return;
        }

        default:
        {
            msgset(response, "error: unknown command type received");
            return;
        }
    }
}