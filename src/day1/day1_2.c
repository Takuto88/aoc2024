#include <stdio.h>
#include "day1_2.h"
#include "liblists.h"

#define LOCATION_ID_LIST_SIZE 1000

int array_count_elements(const int* arr, const int arr_size, const int element_to_count) {
    int count = 0;
    for(int i = 0; i < arr_size; i++) {
        if(arr[i] == element_to_count) {
            count++;
        }
    }

    return count;
}

void solve_day1_2(const char* puzzle_input) {
    lists_t* lists = read_lists(puzzle_input, LOCATION_ID_LIST_SIZE);
    int similarity_score = 0;
    for(int i = 0; i < LOCATION_ID_LIST_SIZE; i++) {
        similarity_score += array_count_elements(lists->right, LOCATION_ID_LIST_SIZE, lists->left[i]) * lists->left[i];
    }
    destroy_list(lists);

    fprintf(stdout, "Day 1_2: Similarity Score: %d\n", similarity_score);
}