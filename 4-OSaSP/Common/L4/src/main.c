#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>

#include "ipc.h"
#include "manager.h"
#include "consumer.h"
#include "producer.h"

queue_t* messages;

sem_t* items;
sem_t* space;
sem_t* mutex;

pid_t producers[CHILD_MAX];
size_t producers_count;

pid_t consumers[CHILD_MAX];
size_t consumers_count;

int main()
{
    init();
    int option;

    while((option = getchar()) != EOF)
    {
        switch(option)
        {
        case 'p':
            create_producer();
            break;

        case 'P':
            remove_producer();
            break;

        case 'c':
            create_consumer();
            break;

        case 'C':
            remove_consumer();
            break;

        case 'q':
            exit(EXIT_SUCCESS);

        default:
            if(option != '\n')
            {
                printf("warning: unknown option `%c` (%d)\n", option, (int) option);
            }

            continue;
        }
    }
}