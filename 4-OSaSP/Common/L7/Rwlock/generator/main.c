#include "record.h"
#include "error_macro.h"

#include <fcntl.h>
#include <unistd.h>

#include <stdio.h>
#include <stdlib.h>

const int count = 10;

int main(void)
{
    int fd = open("input", O_WRONLY | O_CREAT | O_TRUNC, 0644);

    if(fd == -1){
        printerr("open");
    }

    record_t empty = {
        .name = "<empty>",
        .address = "<empty>",
        .semester = 0,
    };

    for(int i = 0; i < count; i++)
    {
        if(write(fd, &empty, sizeof(empty)) != sizeof(empty))
        {
            fprinterr("Failed to write record\n");
        }
    }

    if(close(fd)){
        printerr("close");
    }
}