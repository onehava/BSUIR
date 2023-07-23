#pragma once

#include "ipc.h"

void create_producer();
void remove_producer();

void* producer_hd(__attribute__((unused)) void* arg);

void produce(msg_t* msg);