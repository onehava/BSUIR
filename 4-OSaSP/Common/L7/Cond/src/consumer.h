#pragma once

#include "ipc.h"

void create_consumer();
void remove_consumer();

void* consumer_hd(__attribute__((unused)) void* arg);

void consume(msg_t* msg);