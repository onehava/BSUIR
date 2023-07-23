#pragma once

#include <stdint.h>

typedef struct record {
    char name[80];      // фамилия, имя, отчество
    char address[80];   // ардрес проживания
    uint8_t semester;   // семестр
}
record_t;