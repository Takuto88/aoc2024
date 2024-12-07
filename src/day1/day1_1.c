#include <stdio.h>
#include <stdlib.h>
#include "liblists.h"

#define LOCATION_ID_LIST_SIZE 1000

int comp(const void* left, const void* right) {
    return *(int*) left - *(int*) right;
}

void sort_smallest_to_largest(lists_t* lists) {
    if (lists == NULL) {
        return;
    }

    qsort(lists->left, LOCATION_ID_LIST_SIZE, sizeof(int), comp);
    qsort(lists->right, LOCATION_ID_LIST_SIZE, sizeof (int), comp);
}


void solve_day1_1(const char* puzzle_input) {
    lists_t* lists = read_lists(puzzle_input, LOCATION_ID_LIST_SIZE);
    sort_smallest_to_largest(lists);
    int distance = 0;

    for(int i = 0; i < LOCATION_ID_LIST_SIZE; i++) {
        if (lists->left[i] > lists->right[i]) {
            distance += lists->left[i] - lists->right[i];
            continue;
        }

        distance += lists->right[i] - lists->left[i];
    }

    fprintf(stdout, "Day 1_2: Total distance between lists: %d\n", distance);

    destroy_list(lists);
}