#define _POSIX_SOURCE

#include "manager.h"
#include "ipc.h"

#include <stdio.h>
#include <errno.h>

#include <unistd.h>
#include <stdlib.h>

#include <signal.h>
#include <sys/wait.h>

#include <sys/mman.h>
#include <semaphore.h>

extern queue_t* messages;

extern sem_t* items;
extern sem_t* space;
extern sem_t* mutex;

extern pid_t producers[];
extern size_t producers_count;

extern pid_t consumers[];
extern size_t consumers_count;

static pid_t manager;

void init()
{
    manager = getpid();

    void* ptr = mmap(NULL, 3 * sizeof(sem_t) + sizeof(queue_t), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);

    if(ptr == MAP_FAILED)
    {
        perror("mmap");
        exit(errno);
    }

    items    = ptr + 0;
    space    = ptr + 1 * sizeof(sem_t);
    mutex    = ptr + 2 * sizeof(sem_t);
    messages = ptr + 3 * sizeof(sem_t);

    if(sem_init(mutex, 1, 1) ||
       sem_init(items, 1, 0) ||
       sem_init(space, 1, QUEUE_SIZE))
    {
        perror("sem_init");
        exit(errno);
    }

    atexit(atexit_hd);
}

void atexit_hd()
{
    if(getpid() == manager)
    {
        for(size_t i = 0; i < producers_count; i++){
            kill(producers[i], SIGKILL);
            wait(NULL);
        }

        for(size_t i = 0; i < consumers_count; i++){
            kill(consumers[i], SIGKILL);
            wait(NULL);
        }
    }

    if(sem_destroy(mutex) ||
       sem_destroy(items) ||
       sem_destroy(space))
    {
        perror("sem_destroy");
        exit(errno);
    }
}