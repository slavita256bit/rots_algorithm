#include "utils.h"

#include <stdlib.h>
#include <string.h>
#include <unistd.h>

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