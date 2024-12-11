#ifndef DAY8_H
#define DAY8_H

#define EMPTY_FIELD ('.')

typedef struct {
    char ch;
    int x;
    int y;
} d8_point_t;

typedef struct {
    d8_point_t **contents;
    size_t size;
    size_t capacity;
} d8_point_set_t;

void solve_day8_1(const char* puzzle_input);
void solve_day8_2(const char* puzzle_input);

#endif //DAY8_H
