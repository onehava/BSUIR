#include "generate.h"
#include "index.h"
#include "error_macro.h"

#include <fcntl.h>
#include <unistd.h>

#define EPOCH 15020.0      // Default: 00:00 01 January 1900
#define TODAY 60050.00694  // Default: 00:10 16 April 2023

static double random(double min, double max)
{
    double range = (max - min);
    double div   = (RAND_MAX / range);
    return min   + (rand() / div);
}

void generate(command args)
{
    int count = args.size / sizeof(index_record);
    int status, output;

    output = open(args.name, O_CREAT | O_WRONLY | O_TRUNC, 0644);
    
    if(output == -1){
        printerr("open");
    }

    for(int recno = 0; recno < count; ++recno)
    {
        index_record record = {
            .timestamp = random(EPOCH, TODAY),
            .recno = recno,
        };

        status = write(output, &record, sizeof(record));

        if(status == sizeof(record)){
            continue;
        }

        if(status == -1){
            printerr("write");
        }
        else {
            fprinterr("error: partial index write\n");
        }
    }

    status = close(output);

    if(status){
        printerr("close");
    }
}