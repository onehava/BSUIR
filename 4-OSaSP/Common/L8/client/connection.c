#define _GNU_SOURCE

#include "connection.h"
#include "network.h"
#include "error_macro.h"
#include "message.h"
#include "command.h"
#include "prompt.h"

#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

#include <netdb.h>
#include <arpa/inet.h>

int init_socket(const struct addrinfo* ai)
{
    int server_sock = socket(ai->ai_family, ai->ai_socktype, ai->ai_protocol);   

    if(server_sock == -1){
        perror("socket");
        return -1;
    }

    if(connect(server_sock, ai->ai_addr, ai->ai_addrlen) == -1){
        perror("connect");
        close(server_sock);
        return -1;
    }

    char iptmp[INET6_ADDRSTRLEN]; // todo: make function (1)

    if(!inet_ntop(ai->ai_family, get_address(ai->ai_addr), iptmp, sizeof(iptmp))){
        printerr("inet_ntop");
    }

    return server_sock;
}

void communicate(int server_sock)
{
    run_interactive_prompt(server_sock);
}