#pragma once

#include "ipc.h"

void create_consumer();
void remove_consumer();

void consume(msg_t* msg);