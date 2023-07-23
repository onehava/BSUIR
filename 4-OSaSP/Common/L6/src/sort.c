#include "sort.h"

#include <stdlib.h>

void sort(index_record* sec, int size)
{
    qsort(sec, size / sizeof(index_record), sizeof(index_record), (__compar_fn_t) index_record_comp);
}

// Gap merge method (https://takeuforward.org/data-structure/merge-two-sorted-arrays-without-extra-space/)
//
// Time complexity:  O(size)
// Space Complexity: O(1)

static void index_record_swap(index_record* a, index_record* b)
{
    index_record tmp = *a;
    *a = *b;
    *b = tmp;
}

void merge(index_record* sec_a, index_record* sec_b, int size)
{
    size /= sizeof(index_record);

    for(int gap = size; gap; gap /= 2)
    {
        int i = 0;
        int j = gap;

        while(j < 2 * size)
        {
            if((j < size) && index_record_comp(&sec_a[j], &sec_a[i]) < 0)
            {
                index_record_swap(&sec_a[i], &sec_a[j]);
            }
            else if((j >= size) && (i < size) && index_record_comp(&sec_b[j - size], &sec_a[i]) < 0)
            {
                index_record_swap(&sec_a[i], &sec_b[j - size]);
            }
            else if((j >= size) && (i >= size) && index_record_comp(&sec_b[j - size], &sec_b[i - size]) < 0)
            {
                index_record_swap(&sec_b[i - size], &sec_b[j - size]);
            }

            i++;
            j++;
        }
    }
}