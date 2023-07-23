#include "print.h"

#include <stdio.h>

void rec_print(const record_t* record, unsigned recno)
{
    printf("Record %u\n"
           "\tname: %s\n"
           "\taddress: %s\n"
           "\tsemester: %hhu\n",
           recno, record->name, record->address, record->semester);
}