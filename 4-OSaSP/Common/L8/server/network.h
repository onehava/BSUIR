#pragma once

#include <netdb.h>

struct addrinfo* getaddrinfo_list(const char* port);

void* get_address(const struct sockaddr* sa);