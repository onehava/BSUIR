#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#include "error_macro.h"

#define wrap_function_cond(func, cond, ...)     \
    do {                                        \
        int __STATUS__ = func(__VA_ARGS__);     \
                                                \
        if(cond){                               \
            perror(#func);                      \
            exit(EXIT_FAILURE);                 \
        }                                       \
    }                                           \
    while(0)

#define wrap_function_test(func, ...) \
        wrap_function_cond(func, __STATUS__, __VA_ARGS__)

// Thread

#define thread_create(__newthread, __attr, __start_routine, __arg) \
        wrap_function_test(pthread_create, __newthread, __attr, __start_routine, __arg)

#define thread_join(__th, __thread_return) \
        wrap_function_test(pthread_join, __th, __thread_return)

// Mutex

#define mutex_init(__mutex, __mutexattr) \
        wrap_function_test(pthread_mutex_init, __mutex, __mutexattr)

#define mutex_destroy(__mutex) \
        wrap_function_test(pthread_mutex_destroy, __mutex)


#define mutex_lock(__mutex) \
        wrap_function_test(pthread_mutex_lock, __mutex)

#define mutex_unlock(__mutex) \
        wrap_function_test(pthread_mutex_unlock, __mutex)

// Barrier

#define barrier_init(__barrier, __attr, __count) \
        wrap_function_test(pthread_barrier_init, __barrier, __attr, __count)

#define barrier_destroy(__barrier) \
        wrap_function_test(pthread_barrier_destroy, __barrier)

#define barrier_wait(__barrier) \
        wrap_function_cond(pthread_barrier_wait, __STATUS__ && __STATUS__ != PTHREAD_BARRIER_SERIAL_THREAD, __barrier)
