#include "cube.h"
#include "set.h"
#include "colors.h"

Cube INVALID_CUBE = {-1, 0};

Cube cube_create(int n, ull combined_value) {
    return (Cube){n, combined_value};
}

Cube cube_create_from_int(int n, int value) {
    ull combined_value = 0, i = (n - 1) * 2;
    while (value > 0) {
        combined_value |= ((ull)(value & 1) << i);
        value >>= 1;
        i -= 2;
    }
    return (Cube){n, combined_value};
}

Cube cube_convert_y2x(Cube cube) {
    if (cube_is_invalid(cube))
        return INVALID_CUBE;

    ull y = cube_get_ys(cube);

    cube.value ^= y;
    cube.value |= y << 1;

    return cube;
}

ull both_default_only_mask(Cube a, Cube b) {
    return (DEFAULT_MASK ^ ((cube_special_only(a) | cube_special_only(b)) >> 1)) & cube_n_mask(a);
}

ull cube_n_mask(Cube cube) {
    return (ALL_CUBE_MASK << (cube.n * 2)) ^ ALL_CUBE_MASK;
}

/*
output Y in on default positions
*/
ull cube_get_ys(Cube cube) {
    return ((cube.value & DEFAULT_MASK) & ((cube.value & SPECIAL_MASK) >> 1));
}

ull cube_default_only(Cube cube) {
    return (cube.value & DEFAULT_MASK) & ((SPECIAL_MASK ^ cube_special_only(cube)) >> 1);
}

ull cube_special_only(Cube cube) {
    return (cube.value & SPECIAL_MASK);
}

int cube_x_count(Cube cube) {
    if (cube_is_invalid(cube))
        return -1;
    return __builtin_popcountll(cube_special_only(cube));
}

void cube_set_bit(Cube *cube, int bit, int value) {
    bit *= 2;
    cube->value &= (ALL_CUBE_MASK ^ (3ull << (ull)bit));
    cube->value |= (value << bit);
}

int cube_get_bit(Cube cube, int bit) {
    return (int)((cube.value >> (bit * 2ull)) & 3ull);
}

Cube cube_read(FILE *file, int n) {
    Cube result = cube_create(n, 0);
    char *s = calloc(n + 1, sizeof(char));

    if (fscanf(file, "%s", s) == 0) { printf("Incorrect format"); exit(0); }
    if (strlen(s) != n) { printf("Incorrect format"); exit(0); }

    for (int i = 0; i < n; i++) {
        if (s[i] >= '0' && s[i] <= '1')
            cube_set_bit(&result, i, s[i] - '0');
        else if (s[i] == 'x' || s[i] == 'X')
            cube_set_bit(&result, i, X);
    }

    flush_input(file);
    free(s);

    return result;
}

// names - set of strings of vars with length of cube length (NULL if not needed)
void cube_print(FILE *file, Cube cube, char** var_names) {
    for (int i = 0; i < cube.n; i++) {
        int bit = cube_get_bit(cube, i);
        switch (bit) {
            case Y: fprintf(file, "y"); break;
            case X: fprintf(file, "x"); break;
            default: fprintf(file, "%d", bit); break;
        }
    }

    if (!is_empty(var_names)) {
        printf(" " BLUB "(" RESET);
        for (int j = 0; j < cube.n; j++) {
            CubeValue bit = cube_get_bit(cube, j);
            if (bit == X) continue;

            if (cube_get_bit(cube, j)) printf(BLK WHTB);
            else printf(WHT BLKB);

            printf("%s" RESET, var_names[j]);
            printf(BLUB " " RESET);
        }
        printf(BLUB ")" RESET);
    }
}

int cube_cost(Cube cube) {
    return cube.n - cube_x_count(cube);
}

// is a contains b
bool cube_contains(Cube a, Cube b) {
    ull special_a = cube_special_only(a);
    ull special_b = cube_special_only(b);
    if ((special_a | special_b) != special_a)
        return false;

    ull mask = ((special_a ^ SPECIAL_MASK) >> 1);
    ull default_a = cube_default_only(a) & mask;
    ull default_b = cube_default_only(b) & mask;

    if ((default_a ^ default_b) != 0ull)
        return false;

    return true;
}

bool cube_eq(Cube a, Cube b) {
    return a.n == b.n && a.value == b.value;
}

bool cube_is_invalid(Cube a) {
    return cube_eq(a, INVALID_CUBE);
}

// everything can be made inline and a bit more optimal, but this code is quite readable