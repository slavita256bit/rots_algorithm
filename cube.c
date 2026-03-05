#include "cube.h"

Cube INVALID_CUBE = {-1, 0};

Cube cube_create(int n, ull value) {
    return (Cube){n, value};
}

Cube cube_copy(Cube *cube) {
    return (Cube){cube->n, cube->value};
}

void cube_convert_y2x(Cube *cube) {
    ull y = cube_get_ys(cube);

    cube->value ^= y;
    cube->value |= y << 1;
}

ull both_default_only_mask(Cube *a, Cube *b) {
    return (DEFAULT_MASK ^ ((cube_special_only(a) | cube_special_only(b
        )) >> 1)) & cube_n_mask(a);
}

ull cube_n_mask(Cube *cube) {
    return (ALL_CUBE_MASK << (cube->n * 2)) ^ ALL_CUBE_MASK;
}

/*
output Y in on default positions
*/
ull cube_get_ys(Cube *cube) {
    return ((cube->value & DEFAULT_MASK) & ((cube->value & SPECIAL_MASK) >> 1));
}

ull cube_default_only(Cube *cube) {
    return (cube->value & DEFAULT_MASK) & ((SPECIAL_MASK ^ cube_special_only(cube)) >> 1);
}

ull cube_special_only(Cube *cube) {
    return (cube->value & SPECIAL_MASK);
}

int cube_x_count(Cube *cube) {
    return __builtin_popcountll(cube_special_only(cube));
}

void cube_set_bit(Cube *cube, int bit, int value) {
    bit *= 2;
    cube->value &= (ALL_CUBE_MASK ^ (3ull << (ull)bit));
    cube->value |= (value << bit);
}

int cube_get_bit(Cube *cube, int bit) {
    return (int)((cube->value >> (bit * 2ull)) & 3ull);
}

Cube cube_read(int n) {
    Cube result = cube_create(n, 0);
    char *s = calloc(n + 1, sizeof(char));

    if (scanf("%s", s) == 0) { printf("Incorrect format"); exit(0); }
    if (strlen(s) != n) { printf("Incorrect format"); exit(0); }

    for (int i = 0; i < n; i++) {
        if (s[i] >= '0' && s[i] <= '1')
            cube_set_bit(&result, i, s[i] - '0');
        else if (s[i] == 'x' || s[i] == 'X')
            cube_set_bit(&result, i, X);
    }

    flush_input();
    free(s);

    return result;
}

void cube_print(Cube *cube) {
    for (int i = 0; i < cube->n; i++) {
        int bit = cube_get_bit(cube, i);
        switch (bit) {
            case Y: printf("y"); break;
            case X: printf("x"); break;
            default: printf("%d", bit); break;
        }
    }
}

int cube_cost(Cube *cube) {
    return cube->n - cube_x_count(cube);
}

// a contains b
bool cube_contains(Cube *a, Cube *b) {
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

bool cube_eq(Cube *a, Cube *b) {
    return a->n == b->n && a->value == b->value;
}

// todo make everything inline