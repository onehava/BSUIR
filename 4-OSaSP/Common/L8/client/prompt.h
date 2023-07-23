#pragma once

#include <stdio.h>

#define PROMPT_NESTENCE_MAX 10

void run_script_prompt(FILE* stream, int server_sock, int nestence);

void run_interactive_prompt(int server_sock);