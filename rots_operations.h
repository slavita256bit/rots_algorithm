#pragma once
#include "set.h"

Cube rot_multiply(Cube *a, Cube *b);
Cube rot_union(Cube *a, Cube *b);
Set rot_subtract(Cube *a, Cube *b);
