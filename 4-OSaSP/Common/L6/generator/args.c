#include "args.h"
#include "parse.h"
#include "error_macro.h"

static const char* HELP_MSG 
    = 
    "usage: %s <size> <name>\n"
    "\t<size> - size of index section (in bytes)\n"
    "\t<name> - name of index storage\n";

command parse(int argc, char** argv)
{
    if(argc != 3){
        fprinterr(HELP_MSG, argv[0]);
    }

    command result = {
        .name = argv[2],
        .size = parse_i32(argv[1], PARSE_UNSIGNED, "size"),
    };

    return result;
}