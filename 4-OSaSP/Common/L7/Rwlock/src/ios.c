#include "ios.h"
#include "error_macro.h"

#include <fcntl.h>
#include <unistd.h>

int rec_read(int fd, record_t* dest, int recno)
{
    int status = pread(fd, dest, sizeof(record_t), recno * sizeof(record_t));

    if(status == -1){
        printerr("pread");
    }

    return status == sizeof(record_t);
}

int rec_write(int fd, const record_t* src, int recno)
{
    int status = pwrite(fd, src, sizeof(record_t), recno * sizeof(record_t));

    if(status == -1){
        printerr("pwrite");
    }
    
    return status == sizeof(record_t);
}