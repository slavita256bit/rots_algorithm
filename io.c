#include "io.h"
#include "set.h"

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

char** var_names_read(FILE *file, int cube_length) {
    char** var_names = EMPTY_SET;
    if (!feof(file)) {
        for (int i = 0; i < cube_length; i++) {
            char *str = calloc(MAX_VAR_LENGTH, sizeof(char));
            fscanf(file, "%s", str);
            set_push_force(var_names, str);
        }
    }
    return var_names;
}