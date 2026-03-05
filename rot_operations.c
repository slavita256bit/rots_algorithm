#include "rot_operations.h"

/*
    x: X*X = X
    somes: X*some = some
    y: 1*0 or 0*1 = Y
    just: 1*1 or 0*0 = 1 or 0
*/
Cube rot_multiply(Cube *a, Cube *b) {
    ull special_a = cube_special_only(a), special_b = cube_special_only(b);
    ull default_a = cube_default_only(a), default_b = cube_default_only(b);

    ull x = a->value & b->value & SPECIAL_MASK;

    ull somes_a = (special_a >> 1) & default_b;
    ull somes_b = (special_b >> 1) & default_a;

    ull y = ((default_a ^ default_b) & (((special_a | special_b) ^ SPECIAL_MASK) >> 1));
    if (__builtin_popcountll(y) > 1)
        return INVALID_CUBE;
    y = ((y << 1) | y);

    ull just = default_a & default_b;

    return cube_create(a->n, x | somes_a | somes_b | y | just);
}

Cube rot_union(Cube *a, Cube *b) {
    Cube multiplied = rot_multiply(a, b);

    if (cube_get_ys(&multiplied) != 0)
        return INVALID_CUBE;

    return multiplied;
}

/*
    X in b = Z
    1#1 or 0#0 = Z
    1#0 or 0#1 = Y
    X in a -> inv(b) -> they generate "variations"
*/
Set rot_subtract(Cube *a, Cube *b) {
    ull special_a = cube_special_only(a), special_b = cube_special_only(b);
    ull default_a = cube_default_only(a), default_b = cube_default_only(b);

    ull y = (default_a ^ default_b) & both_default_only_mask(a, b);
    if (y > 0)
        return set_init_with(cube_copy(a));

    ull z = special_b | (((default_a ^ default_b ^ DEFAULT_MASK) & both_default_only_mask(a, b)) << 1);
    if (__builtin_popcountll(z) == a->n)
        return EMPTY_SET;

    ull variations = ((special_a >> 1) ^ default_b) & (special_a >> 1);

    ull z_inv = (z ^ SPECIAL_MASK) & cube_n_mask(a); // all positions that not Z are variations

    Set output = set_init();
    while (z_inv) { // for each bit with value 1
        int pos = __builtin_ctz(z_inv) - 1; // get last bit pos (and align it from special to default pos)
        ull current_variation_mask = 3ull << pos;

        ull new_cube = (a->value & (current_variation_mask ^ ALL_CUBE_MASK)) | (variations & current_variation_mask);
        set_add(&output, cube_create(a->n, new_cube));

        z_inv ^= z_inv & -z_inv; // delete last bit
    }

    return output;
}
/*
tests

110xx
x1000
=====
1101x
110x1

01x00
011x0

011x0
01x00
=====
01110
*/