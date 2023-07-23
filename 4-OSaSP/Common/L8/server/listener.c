#define _GNU_SOURCE

#include "listener.h"
#include "command.h"
#include "session.h"
#include "executor.h"
#include "unimsg.h"

#include <stdio.h>
#include <errno.h>

#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#include <pthread.h>

#define print_norm(...) fprintf(stdout, "[listener] " __VA_ARGS__)
#define print_error(...) fprintf(stderr, "[listener] " __VA_ARGS__)

void* listen_socket_hd(void* arguments)
{
    int client_sock = *((int*) arguments);
    free(arguments);

    print_norm("listen sock=%d\n", client_sock);

    command_container_t container;
    command_t command;
    session_state_t state;
    message_t response;
    int count;
    int error;

    if(init_state(&state)){
        msgset(&response, "internal error: can't init server directory");
        goto halt;
    }

    while(state.runs)
    {
        print_norm("wait command from sock=%d\n", client_sock);

        count = read(client_sock, &container, sizeof(container));

        if(count == 0){
            goto exit;
        }

        if(count == -1){
            print_error("read(): %s\n", strerror(errno));
            goto exit;
        }

        error = unpack_container(&command, &container);

        if(error){
            msgset(&response, "internal error: wrong command structure");
            goto halt;
        }
        
        execute(&command, &state, &response);

        if(response.size){
            count = write(client_sock, response.data, response.size);

            if(count == -1){
                print_error("write(): %s\n", strerror(errno));
                goto exit;
            }
            else {
                response.size = 0;
                continue;  
            }
        }
    }

exit:
    print_norm("disconnected sock=%d\n", client_sock);
    close(client_sock);
    return NULL;

halt:
    write(client_sock, response.data, response.size);
    goto exit;
}

void listen_socket(int client_sock)
{
    int* arguments = malloc(sizeof(int));
    *arguments = client_sock;

    pthread_t tid;
    int error = pthread_create(&tid, NULL, listen_socket_hd, arguments);

    if(error){
        print_error("pthread_create(): %s\n", strerror(error));
        close(client_sock);
        return;
    }
}

#undef print_norm
#undef print_error