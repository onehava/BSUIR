#define _GNU_SOURCE

#include "server.h"
#include "network.h"
#include "listener.h"
#include "error_macro.h"

#include <stdio.h>
#include <unistd.h>

#include <netdb.h>
#include <arpa/inet.h>

#define print_norm(...) fprintf(stdout, "[server] " __VA_ARGS__)
#define print_error(...) fprintf(stderr, "[server] " __VA_ARGS__)

int init_socket(const struct addrinfo* ai) 
{
    int server_sock = socket(ai->ai_family, ai->ai_socktype, ai->ai_protocol);
    
    if(server_sock == -1){
        perror("socket");
        return -1;
    }

    const int true = 1;
  
    if(setsockopt(server_sock, SOL_SOCKET, SO_REUSEADDR, &true, sizeof(true))){
        perror("setsockopt");
        return -1;
    }

    if(bind(server_sock, ai->ai_addr, ai->ai_addrlen)){
        perror("bind");
        close(server_sock);
        return -1;
    }

    if(listen(server_sock, SOMAXCONN)){
        perror("listen");
        close(server_sock);
        return -1;
    }

    char iptmp[INET6_ADDRSTRLEN]; // todo: make function (1)

    if(!inet_ntop(ai->ai_family, get_address(ai->ai_addr), iptmp, sizeof(iptmp))){
        printerr("inet_ntop");
    }

    print_norm("server initialized on %s\n", iptmp);

    return server_sock;
}

void serve_socket(int server_sock)
{
    for(;;){
        struct sockaddr_storage addr;
        socklen_t len = sizeof(addr);

        int client_sock = accept(server_sock, (struct sockaddr_in*) &addr, &len);

        if(client_sock == -1){
            perror("accept");
            continue;
        }

        char iptmp[INET6_ADDRSTRLEN]; // todo: make function (2)

        if(!inet_ntop(addr.ss_family, get_address((struct sockaddr*) &addr), iptmp, sizeof(iptmp))){
            printerr("inet_ntop");
        }

        print_norm("accepted new connection %s (sock=%d)\n", iptmp, client_sock);

        listen_socket(client_sock);
    }
}

#undef print_norm
#undef print_error