#pragma once
#define ull unsigned long long

#define max(a, b) ((a) > (b) ? (a) : (b))
#define min(a, b) ((a) < (b) ? (a) : (b))
#define max_related(a_compare, b_compare, a_result, b_result) ((a_compare) > (b_compare) ? (a_result) : (b_result))

int* random_not_eq(int n, int from, int to);
int random_number(int from, int to);
