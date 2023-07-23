#include "args.h"
#include "scan.h"
#include "index.h"
#include "error_macro.h"

#include <unistd.h>
#include <sys/stat.h>

void test(command args)
{
    struct stat st;

    if(stat(args.name, &st) == -1){
        printerr("stat");
    }

    int index_size = sizeof(index_record);

    if(!st.st_size || st.st_size % index_size){
        fprinterr("error: <name> size must be a multiply of index size (%d)\n", index_size);
    }
}

int main(int argc, char** argv)
{
    command args = parse(argc, argv);
    test(args);
    scan(args);
}