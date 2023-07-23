#pragma once

#include <stdint.h>

#define PARSE_SIGNED 0
#define PARSE_UNSIGNED 1

// Parses string to i32 as signed/unsigned integer
int32_t parse_i32(const char* src, int is_unsigned, const char* var_name);

// Parses string to i64 as signed/unsigned integer
int64_t parse_i64(const char* src, int is_unsigned, const char* var_name);