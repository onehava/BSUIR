#include "scan.h"
#include "index.h"
#include "error_macro.h"

#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>

static index_record next(int fd)
{
    int status;
    index_record record;

    status = read(fd, &record, sizeof(record));

    if(status == -1){
        printerr("read");
    }
    else if(status != sizeof(record)){
        fprinterr("error: partial index read\n");
    }

    return record;
}

void scan(command args)
{
    int fd;
    struct stat st;

    fd = open(args.name, O_RDONLY);

    if(fd == -1){
        printerr("open");
    }

    if(fstat(fd, &st) == -1){
        printerr("fstat");
    }

    int count = st.st_size / sizeof(index_record);

    index_record prev, curr;
    curr = next(fd);

    for(int i = 1; i < count; i++)
    {
        prev = curr;
        curr = next(fd);

        if(index_record_comp(&prev, &curr) >= 0){
            printf("Ascending order violation:\n"
                   "\tindex: %d -> %d\n"
                   "\ttimestamp: %lf -> %lf\n"
                   "\trecno: %d -> %d\n",
                   i - 1, i, 
                   prev.timestamp, curr.timestamp,
                   prev.recno, curr.recno);
        }
    }

    close(fd);
}