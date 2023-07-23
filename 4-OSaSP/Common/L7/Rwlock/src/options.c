#define _GNU_SOURCE

#include "options.h"

#include "ios.h"
#include "lock.h"
#include "print.h"
#include "record.h"
#include "compare.h"

#include "error_macro.h"

#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>

#include <stdio.h>
#include <stdlib.h>

const int npos = -1;

extern int fd;
static record_t rec_tmp;
static record_t rec_mod;
static int recno = npos;

void option_lst()
{
    record_t record;

    rlock(fd);

    for(int number = 0; rec_read(fd, &record, number); number++)
    {
        rec_print(&record, number);
    }

    ulock(fd);
}

void option_get()
{
    record_t record;
    int number;

    if(!scanf("%d", &number)){
        printf("Failed to read record number (int)\n");
        return;
    }
    
    if(number < 0){
        printf("Record number must be positive\n");
        return;
    }

    rec_rlock(fd, number);

    if(!rec_read(fd, &record, number)){
        printf("Failed to read record with number %d\n", number);
    }
    else {
        recno = number;
        rec_tmp = record;
        printf("Record has been successfully read\n");
    }

    rec_ulock(fd, recno);
}

void option_mod()
{
    int option;

    if(recno == npos){
        printf("There is nothing to modify\n");
        return;
    }

    printf("Modification usage:\n"
           "\tn <str>   set name\n"
           "\ta <str>   set address\n"
           "\ts <num>   set semester\n"
           "\tv         view\n"
           "\tu         undo\n"
           "\tq         quit\n");

    rec_mod = rec_tmp;

    while((option = getchar()) != EOF)
    {
        switch(option)
        {
            case 'n':
                if(!scanf("%80s", rec_mod.name)){
                    printf("Failed to read student's name (char[80])\n");
                }
                break;

            case 'a':
                if(!scanf("%80s", rec_mod.address)){
                    printf("Failed to read student's address (char[80])\n");
                }
                break;

            case 's':
                if(!scanf("%hhu", &rec_mod.semester)){
                    printf("Failed to read student's semester (uint8)\n");
                }
                break;

            case 'v':
                rec_print(&rec_mod, recno);
                break;

            case 'u':
                rec_mod = rec_tmp;
                break;

            case 'q':
                return;

            default:
                if(option != 10){
                    printf("unknown command %c (%d)\n", option, option);
                    break;
                }
        }
    }
}

void option_put()
{
    record_t record;

    rec_wlock(fd, recno);

    rec_read(fd, &record, recno);

    if(rec_comparator(&record, &rec_tmp))
    {
        rec_tmp = record;
        
        printf("Record has been modified since last get\n"
               "Please, try again\n");
    }
    else if(!rec_write(fd, &rec_mod, recno))
    {
        recno = npos;

        printf("Record position has been destroyed\n"
               "Current state reset\n");
    }
    else {
        printf("Record has been successfully written\n");
    }

    rec_ulock(fd, recno);
}