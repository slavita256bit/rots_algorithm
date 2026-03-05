#include "io.h"

void flush_input() {
    scanf("%*[^\n]");
    scanf("%*c");
}

int read_int() {
    int x;
    scanf("%d", &x);
    flush_input();
    return x;
}