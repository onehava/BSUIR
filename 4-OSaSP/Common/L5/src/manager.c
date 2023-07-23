#define _POSIX_SOURCE

#include "manager.h"
#include "ipc.h"

#include <stdio.h>
#include <errno.h>

#include <stdlib.h>

#include <pthread.h>
#include <semaphore.h>

extern queue_t messages;

extern sem_t items;
extern sem_t space;
extern sem_t mutex;

extern pthread_t producers[];
extern size_t producers_count;

extern pthread_t consumers[];
extern size_t consumers_count;

void init()
{
    if(sem_init(&mutex, 1, 1) ||
       sem_init(&items, 1, 0) ||
       sem_init(&space, 1, QUEUE_SIZE))
    {
        perror("sem_init");
        exit(errno);
    }

    atexit(atexit_hd);
}

void atexit_hd()
{
    if(sem_destroy(&mutex) ||
       sem_destroy(&items) ||
       sem_destroy(&space))
    {
        perror("sem_destroy");
        exit(errno);
    }
}