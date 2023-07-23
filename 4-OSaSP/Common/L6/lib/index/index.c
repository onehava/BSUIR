#include "index.h"

#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>

#include "error_macro.h"

int index_record_comp(const index_record* p1, const index_record* p2)
{
    index_record x = *((index_record*) p1);
    index_record y = *((index_record*) p2);

    if(x.timestamp == y.timestamp){
        // compare recno
        return x.recno - y.recno;
    }
    else{
        return (x.timestamp < y.timestamp ? -1 : +1);
    }
}