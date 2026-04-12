#include "checker.h"
#define MAX_COMBINATIONS_COUNT 24

bool verify_function(const char *filename, Cubeset optimized_function) {
    FILE *file = fopen(filename, "r");
    int cube_length = read_int(file);
    Cubeset L = cubeset_read(file, read_int(file), cube_length);
    Cubeset N = cubeset_read(file, read_int(file), cube_length);
    fclose(file);

    bool result = true;

    // printf("%-*s | F | OPTIMIZED\n", cube_length, "X");

    int combinations = (1 << cube_length);
    for (int i = 0; i < combinations; i++) {
        Cube cube = cube_create_from_int(cube_length, i);
        // cube_print(stdout, cube); //todo print in debug mode

        bool correct_value = cubeset_contains(L, cube);
        bool is_any = cubeset_contains(N, cube);

        bool optimized_value = cubeset_contains(optimized_function, cube);

        // printf(" | %c | %d\n", (is_any ? '*' : (correct_value ? '1' : '0')), optimized_value);

        if (!is_any && optimized_value != correct_value)
            result = false;
    }

    printf("\noptimized_function (%d):\n", size(optimized_function));
    cubeset_print(stdout, optimized_function);
    printf("\n");

    set_free(L);
    set_free(N);

    return result;
}

void generate_test(const char *filename) {
    FILE *file = fopen(filename, "w");
    int cube_length = random_number(2, 10); // todo test on bigger values after optimizations (or cutting max combinations)
    int combinations = (1 << cube_length);
    int l_and_n_len = random_number(0, min(MAX_COMBINATIONS_COUNT, combinations - 1));
    int l_len = random_number(0, l_and_n_len);
    int n_len = l_and_n_len - l_len;
    int *L_and_N = random_not_eq(l_and_n_len, 0, combinations - 1);

    fprintf(file, "%d\n", cube_length);
    fprintf(file, "%d\n", l_len);
    for (int i = 0; i < l_len; i++) {
        Cube cube = cube_create_from_int(cube_length, L_and_N[i]);
        cube_print(file, cube);
        fprintf(file, "\n");
    }
    fprintf(file, "%d\n", n_len);
    for (int i = 0; i < n_len; i++) {
        Cube cube = cube_create_from_int(cube_length, L_and_N[i + l_len]);
        cube_print(file, cube);
        fprintf(file, "\n");
    }

    free(L_and_N);
    fclose(file);
}

void generate_test_with_x(const char *filename) {
    //todo generator
}