#pragma once

#include <stdio.h>
#include <stdlib.h>

// Prints error message and exit
#define fprinterr(...) do { fprintf(stderr, __VA_ARGS__); exit(EXIT_FAILURE); } while(0)

// Prints error meaning and exit
#define  printerr(msg) do { perror(msg); exit(1); } while(0)