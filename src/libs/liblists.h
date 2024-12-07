#ifndef AOC_2024_LIBLISTS_H
#define AOC_2024_LIBLISTS_H
typedef struct {
    int* left;
    int* right;
} lists_t;

lists_t* read_lists(const char* puzzle_input, int list_length);
void destroy_list(lists_t* lists);

#endif //AOC_2024_LIBLISTS_H
