#define _GNU_SOURCE

#include "consumer.h"

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

extern pthread_t consumers[];
extern size_t consumers_count;

void create_consumer()
{
    if(consumers_count == CHILD_MAX - 1)
    {
        fprintf(stderr, "error: CHILD_MAX limit has been reached for consumers\n");
        return;
    }

    int error = pthread_create(&consumers[consumers_count], NULL, consumer_hd, NULL);

    if(error)
    {
        perror("pthread_create");
        exit(error);
    }

    ++consumers_count;
}

_Noreturn void* consumer_hd(__attribute__((unused)) void* arg)
{
    msg_t message;
    pthread_t tid = pthread_self();

    while(1)
    {
        if(pthread_mutex_lock(&mutex))
        {
            perror("pthread_mutex_lock");
            exit(errno);
        }

        while(messages.size == 0){
            pthread_cond_wait(&can_consume, &mutex);
        }

        pop_msg(&messages, &message);
        size_t count = messages.pop_count;

        if(pthread_cond_signal(&can_produce))
        {
            perror("pthread_cond_signal");
            exit(errno);
        }

        if(pthread_mutex_unlock(&mutex))
        {
            perror("pthread_mutex_unlock");
            exit(errno);
        }

        consume(&message);

        printf("pop count: %zu\n"
               "consumer %lu\n"
               "message:\n"
               "\ttype: %d\n"
               "\thash: %hu\n"
               "\tsize: %d\n\n",
               count, tid, message.type, message.hash, message.size);

        sleep(5);
    }
}

void remove_consumer()
{
    if(consumers_count == 0)
    {
        fprintf(stderr, "error: there are no consumers to remove\n");
        return;
    }

    pthread_t tid = consumers[--consumers_count];
    pthread_cancel(tid);
    pthread_join(tid, NULL);
}

void consume(msg_t* msg)
{
    word msg_hash = msg->hash;
    msg->hash = 0;
    word cur_hash = hash(msg);

    if(msg_hash != cur_hash){
        fprintf(stderr, "error: corrupted message consumed: %d -> %d\n", msg_hash, cur_hash);
    }

    msg->hash = msg_hash;
}