#include "ipc.h"

// source: http://www.cse.yorku.ca/~oz/hash.html
word hash(const msg_t* msg)
{
    word hash = 5381;

    for(size_t i = 0; i < sizeof(msg_t); i++){
        hash = ((hash << 5) + hash) + ((byte*) msg)[i];
    }

    return hash;
}

int put_msg(queue_t* queue, msg_t* msg)
{
    if(queue->size == QUEUE_SIZE){
        return -1;
    }

    queue->messages[queue->tail] = *msg;
    ++queue->tail;
    ++queue->size;
    ++queue->put_count;

    if(queue->tail == QUEUE_SIZE){
        queue->tail = 0;
    }

    return 0;
}

int pop_msg(queue_t* queue, msg_t* msg)
{
    if(queue->size == 0){
        return -1;
    }

    *msg = queue->messages[queue->head];
    ++queue->head;
    --queue->size;
    ++queue->pop_count;

    if(queue->head == QUEUE_SIZE){
        queue->head = 0;
    }

    return 0;
}