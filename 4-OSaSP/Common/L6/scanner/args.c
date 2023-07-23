#include "args.h"
#include "error_macro.h"

static const char* HELP_MSG 
    = 
    "usage: %s <name>\n"
    "\t<name> - name of index storage\n";

command parse(int argc, char** argv)
{
    if(argc != 2){
        fprinterr(HELP_MSG, argv[0]);
    }

    command result = {
        .name = argv[1],
    };

    return result;
}