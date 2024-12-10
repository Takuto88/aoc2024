#include <stddef.h>

#ifndef DAY5_H
#define DAY5_H

typedef struct {
  long high;
  long low;
} ordering_rule_t;

typedef struct {
  long *updates;
  long middle;
  size_t updates_size;
} page_update_t;

void solve_day5_1(const char* puzzle_input);

#endif //DAY5_H
