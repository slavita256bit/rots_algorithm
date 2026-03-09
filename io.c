#include "io.h"

void flush_input(FILE *file) {
    if (file != stdin)
        return;

    scanf("%*[^\n]");
    scanf("%*c");
}

int read_int(FILE *file) {
    int x;
    fscanf(file, "%d", &x);
    flush_input(file);
    return x;
}