#define _GNU_SOURCE

#include <stdio.h>
#include <errno.h>

#include <stdlib.h>
#include <unistd.h>

#include "child.h"

#define log(...) fprintf(stderr, __VA_ARGS__)

static data_t data;
static stat_t stat;

static unsigned int counter;

void setup_state(struct itimerval* timer){
    // установить обработчик atexit
    if(atexit(atexit_hd)){
        log("error: failed to set atexit handler\n");
        exit(EXIT_FAILURE);
    }

    // установить обработчик сигналов родителя
    struct sigaction action;
    action.sa_flags = SA_SIGINFO;
    action.sa_sigaction = extern_hd;

    if(sigaction(SIGUSR1, &action, NULL)){
        perror("sigaction");
        exit(errno);
    }

    // установить обработчик сигнала таймера процесса
    if(signal(SIGALRM, &repeat_hd) == SIG_ERR){
        perror("signal");
        exit(errno);
    }

    // установить таймер
    timer->it_value.tv_sec = 0;
    timer->it_value.tv_usec = TIMER_RATE;
    timer->it_interval.tv_sec = 0;
    timer->it_interval.tv_usec = TIMER_RATE;

    if(setitimer(ITIMER_REAL, timer, NULL)){
        perror("setitimer");
        exit(errno);
    }
}

void reset_state(){
    counter = 0;

    stat._00 = 0;
    stat._01 = 0;
    stat._10 = 0;
    stat._11 = 0;
}

void print_state(){ 
    // todo: посимвольный вывод
    printf("ppid = %d, pid = %d, stat = {%zu, %zu, %zu, %zu}\n",
            getppid(), getpid(), stat._00, stat._01, stat._10, stat._11);

    // сообщить родительскому процессу о завершении вывода статистики.
    msg_end_print();
}

void repeat_hd(__attribute__((unused)) int sig){
    // SIGALRM
    switch(2 * (data.A == 0) + (data.B == 0)){
        case 0:
            stat._00++;
            break;
        case 1:
            stat._01++;
            break;
        case 2:
            stat._10++;
            break;
        case 3:
            stat._11++;
    }

    counter++;
}

void extern_hd(__attribute__((unused)) int sig, siginfo_t* info, __attribute__((unused)) void* context){
    // SIGUSR1
    switch(info->si_int){
        case MSG_STAT:
            print_state();
            break;

        case MSG_KILL:
            exit(EXIT_SUCCESS);

        default:
            log("error: unknown signal '%d' received\n", info->si_int);
            exit(EXIT_FAILURE);
    }
}

void atexit_hd(){
    printf("Process with pid %d has been terminated\n", getpid());
}

void msg_ask_print(){
    union sigval msg = {MSG_ASK};

    if(sigqueue(getppid(), SIGUSR2, msg)){
        perror("sigqueue");
        exit(errno);
    }
}

void msg_end_print(){
    union sigval msg = {MSG_END};

    if(sigqueue(getppid(), SIGUSR2, msg)){
        perror("sigqueue");
        exit(errno);
    }
}

#pragma GCC push_options
#pragma GCC optimize ("O0")

int main(){
    struct itimerval timer;
    setup_state(&timer);

    while(1){
        data = d00;
        data = d11;

        if(counter == PRINT_WAIT){
            msg_ask_print();
            pause();
            reset_state();
        }
    }
}

#pragma GCC pop_options