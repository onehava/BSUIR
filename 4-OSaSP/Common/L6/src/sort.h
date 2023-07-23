#pragma once

#include "index.h"

// Выполняет сортировку блока sec размером size
void sort(index_record* sec, int size);

// Выполняет слияние последовательных блоков sec_a и sec_b размерами size
void merge(index_record* sec_a, index_record* sec_b, int size);