#define _GNU_SOURCE

#include "args.h"
#include "options.h"
#include "error_macro.h"

#include <stdio.h>
#include <stdlib.h>

#include <fcntl.h>
#include <unistd.h>

int fd;

void init(command args)
{
    fd = open(args.name, O_RDWR);

    if(fd == -1){
        printerr("open");
    }
}

int main(int argc, char** argv)
{
    command args = parse(argc, argv);
    init(args);

    printf("Options:\n"
          "\tl          print records\n"
          "\tg <num>    read record\n"
          "\tm          modify record\n"
          "\tp          put record\n"
          "\tq          quit\n");

    int option;

    while((option = getchar()) != EOF)
    {
        switch(option)
        {
            case 'l':
                option_lst();
                break;

            case 'g':
                option_get();
                break;

            case 'm':
                option_mod();
                break;

            case 'p':
                option_put();
                break;

            case 'q':
                return EXIT_SUCCESS;

            default:
                if(option != 10){
                    printf("unknown command %c (%d)\n", option, option);
                    break;
                }
        }
    }
}
