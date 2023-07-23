#pragma once

#define MESSAGE_MAX 4096

typedef struct {
    int size;
    char data[MESSAGE_MAX];
}
message_t;