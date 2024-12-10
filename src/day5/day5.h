#ifndef DAY5_H
#define DAY5_H
#include <stdbool.h>
#include <stddef.h>

typedef struct {
  long high;
  long low;
} ordering_rule_t;

typedef struct {
  long *updates;
  long middle;
  size_t updates_size;
} page_update_t;

typedef struct {
  ordering_rule_t **rules;
  page_update_t *page_update;
  size_t rules_size;
  bool valid_set;
  bool reordered;
} page_update_and_rules_t;

void solve_day5(const char* puzzle_input, const int part);
#endif //DAY5_H
