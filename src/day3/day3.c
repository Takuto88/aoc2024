#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define WINDOW_SIZE (4)
#define WINDOW_END (WINDOW_SIZE - 1)

typedef struct {
    char* token;
    size_t size;
} token_t;

void move_window(char* window) {
    char* beginning = &window[0];
    char* copy_from = &window[1];
    size_t copy_size = sizeof(char) * WINDOW_END;
    memmove(beginning, copy_from, copy_size);
}

int test_for_number(FILE* fp, char* window) {
    int num_str_idx = 0;
    char num_str[10];
    while(!feof(fp) && num_str_idx < 10) {
        num_str[num_str_idx] = (char) fgetc(fp);
        move_window(window);
        window[WINDOW_END] = num_str[num_str_idx];

        // Check if invalid chars are encountered in the first index
        if (num_str_idx == 0 && (num_str[num_str_idx] < '0' || num_str[num_str_idx] > '9')) {
            break;
        }

        // Check if we hit a comma or a ) to denote the end of a number
        if(num_str_idx > 0 && (num_str[num_str_idx] == ',' || num_str[num_str_idx] == ')')) {
            num_str[num_str_idx] = 0;
            return atoi(num_str);
        }

        // Discard any other character
        if (num_str[num_str_idx] < '0' || num_str[num_str_idx] > '9') {
            break;
        }

        num_str_idx++;
    }

    return 0;
}

void solve_day3_1(const char* puzzle_input) {
    FILE* fp = fopen(puzzle_input, "r");
    if (fp == NULL) {
        fprintf(stderr, "Could not open file at: %s", puzzle_input);
        return;
    }

    int result = 0;
    int index = 0; // Counts only up to the window size
    char window[WINDOW_SIZE];

    while (!feof(fp)) {
        const token_t token_mul = {
            .token = "mul(",
            .size = 4,
        };

        // Read up to 4 chars into a window
        if (index < WINDOW_SIZE) {
            window[index] = (char) fgetc(fp);
            index++;
            continue;
        }

        // Copy the last 3 chars from our window to the beginning to make it slide and append the char to the end
        move_window(window);
        window[WINDOW_END] = (char) fgetc(fp);

        // Check if our window contains "mul("
        if(memcmp(window, token_mul.token, sizeof(char) * token_mul.size) != 0) {
            continue;
        }

        // We have a potential match, but we are at the end of file
        if(feof(fp)) {
            break;
        }

        // We have a potential match - now we need to check if the next char is a first_number.
        int first_number = test_for_number(fp, window);
        // Check if we have a first_number AND if we have not encountered a ) yet
        if(first_number == 0 || window[WINDOW_END] == ')') {
            continue;
        }

        // We can test for a next number
        int second_number = test_for_number(fp, window);
        if(second_number != 0) {
            result += first_number * second_number;
        }
    }

    fclose(fp);

    fprintf(stdout, "Day 3_1: All instructions multiplied are: %d\n", result);
}
