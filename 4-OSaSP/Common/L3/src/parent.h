#pragma once

#define _GNU_SOURCE

#include <sys/types.h>
#include <sys/signal.h>

#include "protocol.h"

#define PROCESS_MAX 1024
#define TIMER_WAIT_SEC 5

// Примитив, используемый для нумерации дочерних процессов.
//
typedef int num_t;

// Составной тип, используемый для хранения сведений о дочернем процессе.
//
typedef struct {
    pid_t pid;
    unsigned char is_mute;
} process_t;

// Объявить начальное состояние родительского процесса.
//
void setup_state();

// Сообщить о текущем состоянии родительского процесса.
//
void print_state();

// Создает новый дочерний процесс
//
void create_process(const char* path);

// Удаляет последний дочерний процесс.
//
void pop_process();

// Удаляет все дочерние процессы.
//
void remove_processes();

// Запрещает процессу выводить статистику.
//
void mute_process(num_t num);

// Запрещает всем процессам выводить статистику.
//
void mute_processes();

// Разрешает процессу выводить статистику.
//
void unmute_process(num_t num);

// Разрешает всем процессам выводить статистику.
//
void unmute_processes();

// Узнать num_t процесса через его pid_t
//
num_t search_process(pid_t pid);

// Отправить сообщение дочернему процессу.
//
void send_message(num_t num, enum parent message);

// Отправить сообщение всем дочерним процессам, за исключением exclude.
//
void broadcast_send_message(num_t exclude, enum parent message);

// Обработчик сигналов дочерних процессов.
//
void extern_hd(__attribute__((unused)) int sig, siginfo_t* info, __attribute__((unused)) void* context);

// Обработчик atexit-сигнала.
//
void atexit_hd();

// Обработчик alarm-сигнала.
//
void ptimer_hd(__attribute__((unused)) int sig);