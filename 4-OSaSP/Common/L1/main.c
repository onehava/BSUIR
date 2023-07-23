// to use BSD function scandir() by GCC
#define _GNU_SOURCE

#include <stdio.h>

#include <getopt.h>
#include <stdlib.h>

#include <string.h>
#include <dirent.h>
#include <linux/limits.h>

// message to print at 'command -h'
const char* HELP_MESSAGE = 
    "Usage: dirwalk [OPTIONS] [DIRECTORY]\n"
    "\n"
    "ARGS:\n"
    "\tDIRECTORY\t[default: .]\n\n"
    "OPTIONS:\n"
    "\t-h\tPrint help information\n\n"
    "\t-l\tPrint links\n\n"
    "\t-d\tPrint directories\n\n"
    "\t-f\tPrint files\n\n"
    "\t-s\tPrint according to LC_COLLATE\n\n";


// state of command args
struct
{
    const char* dir;
    unsigned char l;
    unsigned char d;
    unsigned char f;
    unsigned char s;
}
command;

// parse command
void parse(int argc, char** argv){
    // shut error messages down and return (?)
    opterr = 0;
    
    // parse args through getopt()
    int rez = 0;
    
    while((rez = getopt(argc, argv, "ldfsh")) != -1){
        switch(rez)
        {
            case 'l': command.l = 1; break;
            case 'd': command.d = 1; break;
            case 'f': command.f = 1; break;
            case 's': command.s = 1; break;

            case 'h':
                printf("%s", HELP_MESSAGE);
                exit(0);

            case '?':
                fprintf(stderr, "Unknown option -%c\n", optopt);
                exit(1);
            
            default:
                perror("Not valid command-line arguments");
                exit(1);
        }
    }

    if(optind < argc){
        command.dir = argv[optind];
    }
    else {
        command.dir = ".";
    }
}

int match(unsigned int d_type)
{
    return (command.l && d_type == DT_LNK) ||
           (command.d && d_type == DT_DIR) ||
           (command.f && d_type == DT_REG);
}

// get directory entries
int get_entries(const char* __restrict__ path, struct dirent*** __restrict__ entries)
{
    return scandir(path, entries, 0, command.s ? alphasort : 0);
}

// recursive traversal function
void traversal(const char* path)
{
    DIR* dp;
    struct dirent** entries;
    // struct stat fs;
    char subpath[PATH_MAX];

    if((dp = opendir(path)) == NULL)
    {
        fprintf(stderr, "Unable to read directory '%s'\n", path);
        return;
    }

    int n = get_entries(path, &entries);

    for(int i = 0; i < n; i++)
    {
        struct dirent* entry = entries[i];

        if(!strcmp(entry->d_name, ".") || !strcmp(entry->d_name, ".."))
        {
            continue;
        }

        sprintf(subpath, "%s/%s", path, entry->d_name);

        if(match(entry->d_type))
        {
            printf("%s\n", subpath);
        }

        if(entry->d_type == DT_DIR)
        {
            traversal(subpath);
        }

    }

    closedir(dp);
}

int main(int argc, char** argv)
{
    parse(argc, argv);

    if(!(command.l + command.d + command.f))
    {
        command.l = command.d = command.f = 1;
    }
    
    traversal(command.dir);
}