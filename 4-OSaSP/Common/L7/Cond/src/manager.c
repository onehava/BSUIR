#define _GNU_SOURCE

#include "manager.h"
#include "ipc.h"

#include <stdio.h>
#include <errno.h>

#include <stdlib.h>

#include <pthread.h>
#include <semaphore.h>

extern queue_t messages;

extern pthread_mutex_t mutex;
extern pthread_cond_t can_produce;
extern pthread_cond_t can_consume;

extern pthread_t producers[];
extern size_t producers_count;

extern pthread_t consumers[];
extern size_t consumers_count;

void init()
{
    if(pthread_mutex_init(&mutex, NULL))
    {
        perror("pthread_mutex_init");
        exit(errno);
    }

    if(pthread_cond_init(&can_produce, NULL) || 
       pthread_cond_init(&can_consume, NULL))
    {
        perror("pthread_cond_init");
        exit(errno);
    }

    atexit(atexit_hd);
}

void atexit_hd()
{
    if(pthread_mutex_destroy(&mutex))
    {
        perror("pthread_mutex_destroy");
        exit(errno);
    }

    if(pthread_cond_destroy(&can_produce) ||
       pthread_cond_destroy(&can_consume))
    {
        perror("pthread_cond_destroy");
        exit(errno);
    }
}