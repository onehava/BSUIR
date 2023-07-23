#include "args.h"
#include "parse.h"
#include "error_macro.h"

static const char* HELP_MSG
    =
    "usage: %s <memsize> <sections> <threads> <name>\n"
    "\t<memsize>  - mapped memory size in pages\n"
    "\t<sections> - sections count\n"
    "\t<threads>  - threads count\n"
    "\t<name>     - source name\n";

command parse(int argc, char** argv)
{
    if(argc != 5){
        fprinterr(HELP_MSG, argv[0]);
    }

    command result = {
        .memsize  = parse_i32(argv[1], 1, "<memsize>"),
        .sections = parse_i32(argv[2], 1, "<sections>"),
        .threads  = parse_i32(argv[3], 1, "<threads>"),
        .name     = argv[4],
    };

    return result;
}