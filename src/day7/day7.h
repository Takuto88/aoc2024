#ifndef DAY7_H
#define DAY7_H
#include <stddef.h>

typedef struct {
    long result;
    long *operands;
    size_t operands_size;
} calibration_t;

void solve_day7(const char *puzzle_input, int part);

#endif //DAY7_H
