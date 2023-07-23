#include "args.h"
#include "index.h"
#include "generate.h"
#include "error_macro.h"

#include <unistd.h>

void test(command args)
{
    int index_size = sizeof(index_record);

    if(args.size % index_size){
        fprinterr("error: <size> must be a multiply of index size (%d)\n", index_size);
    }

    if(access(args.name, F_OK) != -1){
        printf("warning: overwrite existing <name>\n");
    }
}

int main(int argc, char** argv)
{
    command args = parse(argc, argv);
    test(args);
    generate(args);
}