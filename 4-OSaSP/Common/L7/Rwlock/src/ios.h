#pragma once

#define _GNU_SOURCE

#include "record.h"

// Считывает запись из файла по ее номеру
int rec_read(int fd, record_t* dest, int recno);

// Помещает запись в файл по ее номеру
int rec_write(int fd, const record_t* src, int recno);