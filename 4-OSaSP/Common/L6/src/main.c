#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/mman.h>

#include <fcntl.h>
#include <sys/stat.h>

#include "args.h"
#include "parse.h"

#include "test_args.h"
#include "sync_macro.h"
#include "error_macro.h"

#include "sort.h"
#include "index.h"

// Global runtime data

pthread_mutex_t mutex;
pthread_barrier_t barrier;

char* memory;

struct
{
    int size;
    int next;
    int section_size;
}
section_map;

// Thread

void concurrent_sort(int num)
{
    int section = num;

    for(;;)
    {
        int size = section_map.section_size;
        int off = section * size;

        #ifdef DEBUG
            printf("[sorter] Поток %d выполняет сортировку блока [%d, %d)\n", 
                    num, off, off + size);
        #endif

        sort((index_record*)(memory + off), size);
        mutex_lock(&mutex);

        if(section_map.next == section_map.size)
        {
            mutex_unlock(&mutex);
            return;
        }

        section = section_map.next++;
        mutex_unlock(&mutex);
    }
}

void concurrent_merge(int num)
{
    int section = num;

    for(;;)
    {
        int size = section_map.section_size;
        int offa = 2 * section * size;
        int offb = offa + size;

        #ifdef DEBUG
            printf("[sorter] Поток %d выполняет слияние блоков [%d, %d) <-> [%d, %d)\n", 
                    num, offa, offa + size, offb, offb + size);
        #endif

        merge((index_record*)(memory + offa), (index_record*)(memory + offb), size);
        mutex_lock(&mutex);

        if(section_map.next == section_map.size)
        {
            mutex_unlock(&mutex);
            return;
        }

        section = section_map.next++;
        mutex_unlock(&mutex);
    }
}

void concurrent_process(int num)
{
    barrier_wait(&barrier);
    concurrent_sort(num);
    barrier_wait(&barrier);

    for(;;)
    {
        barrier_wait(&barrier);

        if(num < section_map.size)
        {
            concurrent_merge(num);
        }

        if(section_map.size == 1)
        {
            barrier_wait(&barrier);
            break;
        }

        barrier_wait(&barrier);
    }
}

void* thread_hd(void* argument)
{
    int num = *((int*) argument);
    free(argument);

    for(;;)
    {
        barrier_wait(&barrier);

        if(!section_map.size){
            break;
        }

        concurrent_process(num);
    }

    return NULL;
}

// Main

void init(command args)
{
    mutex_init(&mutex, NULL);
    barrier_init(&barrier, NULL, args.threads + 1);
}

void destroy()
{
    mutex_destroy(&mutex);
    barrier_destroy(&barrier);
}

int main(int argc, char** argv)
{
    command args = parse(argc, argv);
    test(args);
    init(args);

    if(atexit(destroy)){
        fprinterr("can't set atexit function\n");
    }

    pthread_t threads[args.threads];

    for(int number = 0; number < args.threads; ++number)
    {
        int* argument = malloc(sizeof(int));
        *argument = number;
        thread_create(&threads[number], NULL, thread_hd, argument);
    }

    int fd;
    struct stat st;

    fd = open(args.name, O_RDWR);

    if(fd == -1){
        printerr("open");
    }

    if(fstat(fd, &st)){
        printerr("fstat");
    }

    int section_size = args.memsize / args.sections;

    off_t passed = 0;
    off_t remain = st.st_size;

    for(;;)
    {
        if(!remain){
            section_map.size = 0;
            barrier_wait(&barrier);
            break;
        }
        else {
            section_map.size         = args.sections;
            section_map.next         = args.threads;
            section_map.section_size = section_size;
            barrier_wait(&barrier);
        }

        memory = mmap(NULL, args.memsize, PROT_READ | PROT_WRITE, MAP_SHARED, fd, passed);
    
        if(memory == MAP_FAILED){
            printerr("mmap");
        }
        else {
            passed += args.memsize;
            remain -= args.memsize;
        }

        #ifdef DEBUG        
            printf("[master] Вход в секцию сортировки\n");
        #endif

        barrier_wait(&barrier);
        barrier_wait(&barrier);

        #ifdef DEBUG
            printf("[master] Вход в секцию слияния\n");
        #endif

        while(section_map.size != 1)
        {   
            section_map.size /= 2;
            section_map.next = (args.threads < section_map.size) ? args.threads : section_map.size;

            #ifdef DEBUG
                printf("[master] Вход в итерацию слияния:\n"
                    "\tsection_map.size = %d\n"
                    "\tsection_map.next = %d\n"
                    "\tsection_map.section_size = %d\n", 
                    section_map.size, section_map.next, section_map.section_size);
            #endif

            barrier_wait(&barrier);
            barrier_wait(&barrier);

            section_map.section_size *= 2;
        }

        #ifdef DEBUG
            printf("[master] Данные синхронизируются с диском и выгружаются\n");
        #endif
    
        if(msync(memory, args.memsize, MS_SYNC) == -1){
            printerr("msync");
        }

        if(munmap(memory, args.memsize) == -1){
            printerr("munmap");
        }
    }

    for(int number = 0; number < args.threads; ++number)
    {
        thread_join(threads[number], NULL);
    }
}