#define _POSIX_SOURCE

#include "consumer.h"

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

extern pid_t consumers[];
extern size_t consumers_count;

void create_consumer()
{
    if(consumers_count == CHILD_MAX - 1)
    {
        fprintf(stderr, "error: CHILD_MAX limit has been reached for consumers\n");
        return;
    }

    switch(consumers[consumers_count] = fork()){
        case -1:
            perror("fork");
            exit(errno);

        case +0:
            break;

        default:
            ++consumers_count;
            return;
    }

    pid_t pid = getpid();
    msg_t message;

    while(1)
    {
        sem_wait(items);
        sem_wait(mutex);

        pop_msg(messages, &message);
        size_t count = messages->pop_count;

        sem_post(mutex);
        sem_post(space);

        consume(&message);

        printf("pop count: %zu\n"
               "consumer %d\n"
               "message:\n"
               "\ttype: %d\n"
               "\thash: %hu\n"
               "\tsize: %d\n\n",
               count, pid, message.type, message.hash, message.size);

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

    --consumers_count;
    kill(consumers[consumers_count], SIGKILL);
    wait(NULL);
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