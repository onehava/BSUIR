#include "unimsg.h"

#include <string.h>

int msgcat(message_t* msg, const char* src)
{
    int count = strlen(src);

    if(msg->size + count >= MESSAGE_MAX){
        return -1;
    }

    memcpy(msg->data + msg->size, src, count);
    msg->size += count;
    return 0;
}

int msgset(message_t* msg, const char* src)
{
    int count = strlen(src);

    if(count >= MESSAGE_MAX){
        return -1;
    }

    memcpy(msg->data, src, count);
    msg->size = count;
    return 0;
}