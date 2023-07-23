#include "test_args.h"

// memsize
// sections
// threads
// name

#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>

#include "index.h"
#include "error_macro.h"

static int is_pow(long number, long base)
{
    for(int i = 1; i; i *= base)
    {
        if(i > number){
            return 0;
        }
        else if(i == number){
            return 1;
        }
    }

    return 0;
}

static int is_mux(long number, long divider)
{
    return number && (number % divider == 0);
}

void test(command args)
{
    int page_size = sysconf(_SC_PAGE_SIZE);

    if(!is_mux(args.memsize, page_size)){
        fprinterr("<memsize> must be a multiply of page size (%d)\n", page_size);
    }

    if(!is_pow(args.sections, 2)){
        fprinterr("<sections> must be a power of 2\n");
    }

    if(!args.threads){
        fprinterr("<threads> must be positive\n");
    }

    if(args.sections <= args.threads){
        fprinterr("<sections> must be greater than <threads>\n");
    }

    if(!is_mux(args.memsize, args.sections)){
        fprinterr("<memsize> must be a multiply of <sections>\n");
    }

    int section_size = args.memsize / args.sections;
    int index_size = sizeof(index_record);

    if(!is_mux(section_size, index_size)){
        fprinterr("section size (%d) must be a multiply of index size (%d)\n", section_size, index_size);
    }

    struct stat st;

    if(stat(args.name, &st)){
        printerr("stat");
    }

    if(!is_mux(st.st_size, args.memsize)){
        printf("%ld <-> %d\n", st.st_size, args.memsize);
        fprinterr("<name> size must be a multiply of <memsize>\n");
    }
}