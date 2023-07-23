#pragma once

#define _GNU_SOURCE

#include <sys/time.h>
#include <sys/types.h>
#include <sys/signal.h>

#include "protocol.h"

#define TIMER_RATE 10000
#define PRINT_WAIT 500

// Составной тип данных, представляющий собой неатомарную величину.
//
typedef struct {
    int64_t A, B;
} data_t;

const data_t d00 = {0, 0};
const data_t d11 = {1, 1};

// Составной тип данных, используемый для хранения статистики.
//
typedef struct {
    size_t _00, _01, _10, _11;
} stat_t;

// Сообщить о намерении вывести статистику.
//
void msg_ask_print();

// Сообщить о завершении вывода статистики.
//
void msg_end_print();

// Обработчик alarm-сигнала таймера процесса.
//
void repeat_hd(__attribute__((unused)) int sig);

// Обработчик сигналов родительского процесса.
//
void extern_hd(__attribute__((unused)) __attribute__((unused)) int sig, siginfo_t* info, __attribute__((unused)) __attribute__((unused)) void* context);

// Обработчик atexit-сигнала.
//
void atexit_hd();

// Данная функция устанавливает состояние процесса.
//
void setup_state(struct itimerval* timer);

// Данная функция сбрасывает состояние процесса.
//
void reset_state();

// Данная функция выводит текущее состояние процесса.
//
void print_state();