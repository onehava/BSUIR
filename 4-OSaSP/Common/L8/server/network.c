#define _GNU_SOURCE

#include "network.h"
#include "error_macro.h"

#include <stdlib.h>
#include <string.h>
#include <unistd.h>

struct addrinfo* getaddrinfo_list(const char* port) 
{
    char hostname[512];

    if(gethostname(hostname, sizeof(hostname)) == -1){
        printerr("gethostname");
    }

    struct addrinfo hints = {
        .ai_family = AF_INET,
        .ai_socktype = SOCK_STREAM,
    };

    struct addrinfo* result = NULL;

    int error = getaddrinfo(hostname, port, &hints, &result);

    if(error){
        fprinterr("getaddrinfo: %s\n", gai_strerror(error));
    }

    return result;
}

void* get_address(const struct sockaddr* sa) 
{
    switch(sa->sa_family)
    {
    case AF_INET:
        return &(((struct sockaddr_in*) sa)->sin_addr);

    case AF_INET6:
        return &(((struct sockaddr_in6*) sa)->sin6_addr);

    default:
        return NULL;
    }
}
