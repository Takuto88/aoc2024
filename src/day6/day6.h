#ifndef DAY6_H
#define DAY6_H
#include <stdint.h>
#include <stdbool.h>

typedef enum {
    UP,
    RIGHT,
    DOWN,
    LEFT,
} guard_direction_t;

typedef struct {
    uint16_t x_pos;
    uint16_t y_pos;
    guard_direction_t direction;
    bool has_left;
    bool is_stuck_in_loop;
} guard_t;

typedef struct {
    char **raw_map;
    uint16_t max_x;
    uint16_t max_y;
} map_t;

typedef struct {
    map_t *map;
    guard_t *guard;
} world_t;

void solve_day_6_1(const char* puzzle_input);
void solve_day_6_2(const char* puzzle_input);
#endif //DAY6_H
