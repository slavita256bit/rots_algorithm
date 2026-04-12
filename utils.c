#include "utils.h"

#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <time.h>

#include "cube.h"

int* random_not_eq(int n, int from, int to) { // todo not optimal
    int *a = calloc(n, sizeof(int));
    bool *used = calloc(to - from + 1, sizeof(bool));
    for (int i = 0; i < n; i++) {
        do {
            a[i] = rand() % (to - from + 1) + from;
        } while (used[a[i]]);
        used[a[i]] = true;
    }
    free(used);
    return a;
}

int random_number(int from, int to) {
    return rand() % (to - from + 1) + from;
}

//todo c qsort can be used lol
// adopted https://gist.github.com/adwiteeya3/f1797534506be672b591f465c3366643
void quick_sort_cube(void* arr, int low, int high, void* comp) {
    if (low < high) {
        int pi;
        partition(arr, low, high, Cube, comp, pi);
        quick_sort_cube(arr, low, pi - 1, comp);
        quick_sort_cube(arr, pi + 1, high, comp);
    }
}

// returns pointer after end of time
char* get_time_formated(char* dest, time_t *now) {
    struct tm *now_info = localtime(now);
    int time_len = strftime(dest, FILENAME_MAX, "__%Y_%m_%d__%H_%M_%S", now_info);
    return dest + time_len;
}