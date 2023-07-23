#pragma once

#include <netdb.h>

int init_socket(const struct addrinfo* ai);

void communicate(int server_sock);