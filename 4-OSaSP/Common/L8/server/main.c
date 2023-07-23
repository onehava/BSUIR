#define _GNU_SOURCE

#include "server.h"
#include "network.h"
#include "error_macro.h"

#include <netdb.h>
#include <arpa/inet.h>

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv)
{
    if(argc != 2){
        fprinterr("usage: %s <port>\n", argv[0]);
    }

    struct addrinfo* list = getaddrinfo_list(argv[1]);

    for(struct addrinfo* node = list; node != NULL; node = node->ai_next)
    {
        int server_sock = init_socket(node);

        if(server_sock == -1){
            continue;
        }

        serve_socket(server_sock);
    }
}