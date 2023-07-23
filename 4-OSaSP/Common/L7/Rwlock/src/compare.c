#include "compare.h"

#include <string.h>

int rec_comparator(const record_t* a, const record_t* b)
{
    if(a->semester == b->semester)
    {
        int tmp = strcmp(a->name, b->name);

        if(tmp == 0){
            return strcmp(a->address, b->address);
        }
        else {
            return tmp;
        }
    }
    else if(a->semester > b->semester){
        return 1;
    }
    else {
        return -1;
    }
}