#include "lock.h"
#include "record.h"

#include <fcntl.h>
#include <unistd.h>

#include "error_macro.h"

void lock(int fd, short type)
{
    static struct flock params = {
        .l_whence = SEEK_SET,
        .l_start = 0L,
        .l_len = 0L,
    };

    params.l_type = type;

    if(fcntl(fd, F_SETLKW, &params) == -1){
        printerr("fcntl");
    }
}

void rec_lock(int fd, int recno, short type)
{
    static struct flock params = {
        .l_whence = SEEK_CUR,
        .l_len = sizeof(record_t),
    };

    params.l_type = type;
    params.l_start = recno * sizeof(record_t);

    if(fcntl(fd, F_SETLKW, &params) == -1){
        printerr("fcntl");
    }
}