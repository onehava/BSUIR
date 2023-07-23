#pragma once

#include <sys/types.h>

#define QUEUE_SIZE 128
#define CHILD_MAX 1024

typedef unsigned char byte;
typedef unsigned short word;

typedef struct {
    byte type;
    word hash;
    byte size;
    byte data[256];
} msg_t;

typedef struct {
    size_t put_count;           // счетчик помещенных сообщений
    size_t pop_count;           // счетчик извлеченных сообщений

    size_t head;                // указывает на индекс головы
    size_t tail;                // указывает на индекс хвоста
    size_t size;                // используется для упрощения проверки оставшегося размера
    msg_t messages[QUEUE_SIZE]; // буфер сообщений
} queue_t;

word hash(const msg_t* msg);

int put_msg(queue_t* queue, msg_t* msg);
int pop_msg(queue_t* queue, msg_t* msg);