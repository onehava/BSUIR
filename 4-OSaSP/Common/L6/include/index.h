#pragma once

typedef struct 
{
    double timestamp;   // Modified JD (https://en.wikipedia.org/wiki/Julian_day);
    int recno;          // This number equivalent to index in first indexes table;
} 
index_record;           // Stores second-level db index based on Julian timestamp;

// Compares index_record
int index_record_comp(const index_record* p1, const index_record* p2);

// Tests index_record fd
// int index_header_test(int fd);