#define _GNU_SOURCE

#include "connection.h"
#include "network.h"
#include "error_macro.h"

int main(int argc, char** argv)
{
    if(argc != 3){
        fprinterr("usage: %s <hostname> <port>\n", argv[0]);
    }

    struct addrinfo* list = getaddrinfo_list(argv[1], argv[2]);

    for(struct addrinfo* node = list; node != NULL; node = node->ai_next) 
    {
        int server_sock = init_socket(node);

        if(server_sock < 0){
            continue;
        }

        communicate(server_sock);
        return 0;
    }

    fprinterr("connection to hostname %s is failed\n", argv[1]);
}