#pragma once
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "io.h"
#include "utils.h"

constexpr ull ALL_CUBE_MASK = UINT64_MAX; // like ... 11 11 11 11
constexpr ull SPECIAL_MASK = 0xAAAAAAAAAAAAAAAA; // like ... 10 10 10 10
constexpr ull DEFAULT_MASK = 0x5555555555555555; // like ... 01 01 01 01

/*
    n = -1 indicates that cube cannot exist
    For example: two Y's in multiply or 1 union 0
*/
struct Cube {
    int n;
    ull value; // todo: can be adopted to "inf" variables with list of "chunks", currently max is 64/2=32
};
typedef struct Cube Cube;

extern Cube INVALID_CUBE;

enum CubeValue {
    ZERO = 0,
    ONE = 1,
    X = 2, // 10
    Y = 3, // 11
};
typedef enum CubeValue CubeValue;

Cube cube_create(int n, ull combined_value);
Cube cube_create_from_int(int n, int value);

void cube_set_bit(Cube *cube, int bit, int value);
int cube_get_bit(Cube cube, int bit);
ull cube_n_mask(Cube cube);

Cube cube_convert_y2x(Cube cube);
ull cube_get_ys(Cube cube);
int cube_x_count(Cube cube);

ull both_default_only_mask(Cube a, Cube b);
ull cube_default_only(Cube cube);
ull cube_special_only(Cube cube);

bool cube_contains(Cube a, Cube b);
bool cube_eq(Cube a, Cube b);
bool cube_is_invalid(Cube a);
int cube_cost(Cube cube);

Cube cube_read(FILE *file, int n);
void cube_print(FILE *file, Cube cube, char** var_names);
