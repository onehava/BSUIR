#include "parse.h"

#include <stdio.h>
#include <errno.h>

#include <stdlib.h>
#include <limits.h>
#include <stdint.h>

#define fprinterr(...) do { fprintf(stderr, __VA_ARGS__); exit(EXIT_FAILURE); } while(0)

int32_t parse_i32(const char* src, int is_unsigned, const char* var_name)
{
    if(is_unsigned && (*src == '-' || *src == '+')){
        fprinterr("error: <%s> must be unsigned\n", var_name);
    }

    char* tmp;
    long size = strtol(src, &tmp, 10);

    if(tmp == src){
        fprinterr("error: <%s> isn't integer\n", var_name);
    }

    if(*tmp != '\0'){
        fprinterr("error: <%s> ends with extra characters\n", var_name);
    }

    #if INT_MAX == LONG_MAX
    //1
        if((size == INT32_MAX || size == INT32_MIN) && (errno == ERANGE)){
            fprinterr("error: <%s> outs of range\n", var_name);
        }
    #else
    //2
        if(size > INT32_MAX || size < INT32_MIN){
            errno = ERANGE;
            fprinterr("error: <%s> outs of range\n", var_name);
        }
    #endif

    return size;
}

int64_t parse_i64(const char* src, int is_unsigned, const char* var_name)
{
    if(is_unsigned && (*src == '-' || *src == '+')){
        fprinterr("error: <%s> must be unsigned\n", var_name);
    }

    char* tmp;
    long long size = strtoll(src, &tmp, 10);

    if(tmp == src){
        fprinterr("error: <%s> isn't integer\n", var_name);
    }

    if(*tmp != '\0'){
        fprinterr("error: <%s> ends with extra characters\n", var_name);
    }

    if((size == INT64_MAX || size == INT64_MIN) && (errno == ERANGE)){
        errno = ERANGE;
        fprinterr("error: <%s> outs of range\n", var_name);
    }

    return size;
}