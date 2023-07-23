#define _GNU_SOURCE

#include "producer.h"

#include <stdio.h>
#include <errno.h>

#include <unistd.h>
#include <stdlib.h>

#include <pthread.h>
#include <semaphore.h>

extern queue_t messages;

extern pthread_mutex_t mutex;
extern pthread_cond_t can_produce;
extern pthread_cond_t can_consume;

extern pthread_t producers[];
extern size_t producers_count;

void create_producer()
{
    if(producers_count == CHILD_MAX - 1)
    {
        fprintf(stderr, "error: CHILD_MAX limit has been reached for producers\n");
        return;
    }

    int error = pthread_create(&producers[producers_count], NULL, producer_hd, NULL);

    if(error)
    {
        perror("pthread_create");
        exit(error);
    }

    ++producers_count;
}

_Noreturn void* producer_hd(__attribute__((unused)) void* arg)
{
    msg_t message;
    pthread_t tid = pthread_self();

    while(1)
    {
        produce(&message);

        if(pthread_mutex_lock(&mutex))
        {
            perror("pthread_mutex_lock");
            exit(errno);
        }

        while(messages.size == QUEUE_SIZE){
            pthread_cond_wait(&can_produce, &mutex);
        }

        put_msg(&messages, &message);
        size_t count = messages.put_count;

        if(pthread_cond_signal(&can_consume))
        {
            perror("pthread_cond_signal");
            exit(errno);
        }

        if(pthread_mutex_unlock(&mutex))
        {
            perror("pthread_mutex_unlock");
            exit(errno);
        }

        printf("put count: %zu\n"
               "producer %lu\n"
               "message:\n"
               "\ttype: %d\n"
               "\thash: %hu\n"
               "\tsize: %d\n\n",
               count, tid, message.type, message.hash, message.size);

        sleep(5);
    }
}

void remove_producer()
{
    if(producers_count == 0)
    {
        fprintf(stderr, "error: there are no producers to remove\n");
        return;
    }

    pthread_t tid = producers[--producers_count];
    pthread_cancel(tid);
    pthread_join(tid, NULL);
}

void produce(msg_t* msg)
{
    word size = rand() % 256;
    word type = rand() % 0x8;

    for(word i = 0; i < size; i++){
        msg->data[i] = rand() % 256;
    }

    msg->size = size;
    msg->type = type;
    msg->hash = 0;
    msg->hash = hash(msg);
}