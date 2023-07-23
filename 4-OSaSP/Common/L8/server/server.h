#pragma once

#include <netdb.h>

int init_socket(const struct addrinfo* ai);

void serve_socket(int server_sock);
