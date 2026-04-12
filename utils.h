#pragma once
#include <stdlib.h>
#define ull unsigned long long

#define max(a, b) ((a) > (b) ? (a) : (b))
#define min(a, b) ((a) < (b) ? (a) : (b))
#define max_related(a_compare, b_compare, a_result, b_result) ((a_compare) > (b_compare) ? (a_result) : (b_result))

#define swap(a, b, type) do {         \
        type t = a;                   \
        a = b;                        \
        b = t;                        \
    } while (0);


// adopted https://gist.github.com/adwiteeya3/f1797534506be672b591f465c3366643
#define partition(arr_nt, low, high, type, comp_nt, result) do {    \
        type* _arr = arr_nt;                                        \
        bool(*_comp)(type a, type b) = comp_nt;                     \
        type pivot = _arr[high];                                    \
        int i = (low - 1);                                          \
        for (int j = low; j <= high - 1; j++) {                     \
            if (_comp(_arr[j], pivot))                              \
                swap(_arr[i], _arr[j], type);                       \
        }                                                           \
        swap(_arr[i + 1], _arr[high], type);                        \
        result = (i + 1);                                           \
    } while(0);

#define sort(arr, low, high, comp) _Generic((arr), \
    struct Cube*: quick_sort_cube                  \
    )(arr, low, high, comp)


int* random_not_eq(int n, int from, int to);
int random_number(int from, int to);
void quick_sort_cube(void* arr, int low, int high, void* comp);
char* get_time_formated(char* dest, time_t *time);