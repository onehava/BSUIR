#define _POSIX_SOURCE

#include "producer.h"

#include <stdio.h>
#include <errno.h>

#include <unistd.h>
#include <stdlib.h>

#include <signal.h>
#include <sys/wait.h>

#include <semaphore.h>

extern queue_t* messages;

extern sem_t* items;
extern sem_t* space;
extern sem_t* mutex;

extern pid_t producers[];
extern size_t producers_count;

void create_producer()
{
    if(producers_count == CHILD_MAX - 1)
    {
        fprintf(stderr, "error: CHILD_MAX limit has been reached for producers\n");
        return;
    }

    switch(producers[producers_count] = fork()){
        default:
            producers_count++;
            return;

        case 0:
            srand(getpid());
            break;

        case -1:
            perror("fork");
            exit(errno);
    }

    pid_t pid = getpid();
    msg_t message = {0};

    while(1)
    {
        produce(&message);

        sem_wait(space);
        sem_wait(mutex);

        put_msg(messages, &message);
        size_t count = messages->put_count;

        sem_post(mutex);
        sem_post(items);

        printf("put count: %zu\n"
               "producer %d\n"
               "message:\n"
               "\ttype: %d\n"
               "\thash: %hu\n"
               "\tsize: %d\n\n",
               count, pid, message.type, message.hash, message.size);

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

    --producers_count;
    kill(producers[producers_count], SIGKILL);
    wait(NULL);
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