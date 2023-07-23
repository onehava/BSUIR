#pragma once

#include "ipc.h"

void create_producer();
void remove_producer();

void produce(msg_t* msg);