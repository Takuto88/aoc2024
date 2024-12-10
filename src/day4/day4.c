#include "day4.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


const char *needle_part_1 = "XMAS";
const char *needle_part_2 = "MAS";

void destroy_puzzle(puzzle_t *puzzle) {
    if (puzzle == NULL) {
        return;
    }

    if (puzzle->input == NULL) {
        free(puzzle);
        return;
    }

    for (int i = 0; i < puzzle->num_lines; i++) {
        free(puzzle->input[i]);
    }

    free(puzzle->input);
    free(puzzle);
}

puzzle_t *load_puzzle(FILE *fp) {
    rewind(fp);
    puzzle_t *puzzle = malloc(sizeof(puzzle_t));
    if (puzzle == NULL) {
        return NULL;
    }

    char ch;
    int num_lines = 0, line_length = 0;
    bool first_new_line_encountered = false;

    // This does not include the \n char at the end and that is intentional
    while (!feof(fp)) {
        ch = (char) fgetc(fp);
        if (ch == '\n' || feof(fp)) {
            num_lines++;
            first_new_line_encountered = true;
        }

        if (first_new_line_encountered == false) {
            line_length++;
        }
    }
    rewind(fp);

    puzzle->num_lines = num_lines;
    puzzle->line_length = line_length;

    puzzle->input = malloc(sizeof(char *) * num_lines);
    if (puzzle->input == NULL) {
        free(puzzle);
        return NULL;
    }

    int i = 0;

    // + 2 because of \n and null terminator
    char *line = malloc(sizeof(char) * line_length + 2);
    while (fgets(line, line_length + 2, fp) != NULL) {
        size_t len = strlen(line);
        if (line[len - 1] == '\n') {
            line[len - 1] = '\0';
        }
        puzzle->input[i] = malloc(sizeof(char) * line_length + 2);

        // In case of OOM: Tear down everything we have so far.
        if (puzzle->input[i] == NULL) {
            for (int reverse = i - 1; reverse > 0; reverse--) {
                free(puzzle->input[i]);
            }
            free(puzzle);
            free(line);
            return NULL;
        }
        strcpy(puzzle->input[i], line);
        i++;
    }

    free(line);
    return puzzle;
}

char *get_forward(const puzzle_t *puzzle, const int x, const int y, int size) {
    if (y + size > puzzle->line_length) {
        size = y + size - puzzle->line_length;
    }

    char *str = malloc(sizeof(char) * size + 1);

    strncpy(str, puzzle->input[x] + y, size);
    str[size] = '\0';

    return str;
}

char *get_backward(const puzzle_t *puzzle, const int x, const int y, int size) {
    if (y - (size - 1) < 0) {
        size = size - y;
    }

    char *str = malloc(sizeof(char) * size + 1);
    char *copy_from = &puzzle->input[x][y];

    for (size_t i = 0; i < size; i++) {
        str[i] = *copy_from;
        copy_from--;
    }

    str[size] = '\0';

    return str;
}

char *get_top(const puzzle_t *puzzle, const int x, const int y, int size) {
    if (x - (size - 1) < 0) {
        size = size - (x - size) * -1;
    }

    char *str = malloc(sizeof(char) * size + 1);
    for (size_t i = 0; i < size; i++) {
        const char *copy_from = &puzzle->input[x - i][y];
        str[i] = *copy_from;
    }

    str[size] = '\0';

    return str;
}

char *get_below(const puzzle_t *puzzle, const int x, const int y, int size) {
    if (x + size > puzzle->num_lines) {
        size = puzzle->num_lines - x;
    }

    char *str = malloc(sizeof(char) * size + 1);
    for (size_t i = 0; i < size; i++) {
        const char *copy_from = &puzzle->input[x + i][y];
        str[i] = *copy_from;
    }

    str[size] = '\0';

    return str;
}

char *get_diag_top_left(const puzzle_t *puzzle, const int x, const int y, int size) {
    int max_size_x = size;
    if (x - (size - 1) < 0) {
        max_size_x = (size - (x - size) * -1) + 1;
    }

    int max_size_y = size;
    if (y - (size - 1) < 0) {
        max_size_y = (size - (y - size) * -1) + 1;
    }

    if (max_size_x < size || max_size_y < size) {
        size = max_size_x < max_size_y ? max_size_x : max_size_y;
    }

    char *str = malloc(sizeof(char) * size + 1);
    for (size_t i = 0; i < size; i++) {
        const char *copy_from = &puzzle->input[x - i][y - i];
        str[i] = *copy_from;
    }

    str[size] = '\0';
    return str;
}

char *get_diag_top_right(const puzzle_t *puzzle, const int x, const int y, int size) {
    int max_size_x = size;
    if (x - (size - 1) < 0) {
        max_size_x = (size - (x - size) * -1) + 1;
    }

    int max_size_y = size;
    if (y + size > puzzle->line_length) {
        max_size_y = y + size - puzzle->line_length;
    }

    if (max_size_x < size || max_size_y < size) {
        size = max_size_x < max_size_y ? max_size_x : max_size_y;
    }

    char *str = malloc(sizeof(char) * size + 1);
    for (size_t i = 0; i < size; i++) {
        const char *copy_from = &puzzle->input[x - i][y + i];
        str[i] = *copy_from;
    }

    str[size] = '\0';
    return str;
}

char *get_diag_bottom_left(const puzzle_t *puzzle, const int x, const int y, int size) {
    int max_size_x = size;
    if (x + size > puzzle->num_lines) {
        max_size_x = puzzle->num_lines - x;
    }

    int max_size_y = size;
    if (y - (size - 1) < 0) {
        max_size_y = (size - (y - size) * -1) + 1;
    }

    if (max_size_x < size || max_size_y < size) {
        size = max_size_x < max_size_y ? max_size_x : max_size_y;
    }

    char *str = malloc(sizeof(char) * size + 1);
    for (size_t i = 0; i < size; i++) {
        const char *copy_from = &puzzle->input[x + i][y - i];
        str[i] = *copy_from;
    }

    str[size] = '\0';
    return str;
}

char *get_diag_bottom_right(const puzzle_t *puzzle, const int x, const int y, int size) {
    int max_size_x = size;
    if (x + size > puzzle->num_lines) {
        max_size_x = puzzle->num_lines - x;
    }

    int max_size_y = size;
    if (y + size > puzzle->line_length) {
        max_size_y = y + size - puzzle->line_length;
    }

    if (max_size_x < size || max_size_y < size) {
        size = max_size_x < max_size_y ? max_size_x : max_size_y;
    }

    char *str = malloc(sizeof(char) * size + 1);
    for (size_t i = 0; i < size; i++) {
        const char *copy_from = &puzzle->input[x + i][y + i];
        str[i] = *copy_from;
    }

    str[size] = '\0';
    return str;
}

void solve_day4_1(const char *puzzle_input) {
    FILE *fp = fopen(puzzle_input, "r");
    if (fp == NULL) {
        fprintf(stderr, "Could not load puzzle input file: %s\n", puzzle_input);
        return;
    }

    puzzle_t *puzzle = load_puzzle(fp);

    int found = 0;
    const int size = 4;
    char* (*get_string[8])(const puzzle_t*, int, int, int);
    get_string[0] = get_forward;
    get_string[1] = get_backward;
    get_string[2] = get_top;
    get_string[3] = get_below;
    get_string[4] = get_diag_top_left;
    get_string[5] = get_diag_top_right;
    get_string[6] = get_diag_bottom_left;
    get_string[7] = get_diag_bottom_right;

    int found_using[8] = { 0 };

    for (int x = 0; x < puzzle->num_lines; x++) {
        for (int y = 0; y < puzzle->line_length; y++) {

            for (int f_ptr_idx = 0; f_ptr_idx < 8; f_ptr_idx++) {
                char* str = get_string[f_ptr_idx](puzzle, x, y, size);

                if (strcmp(str, needle_part_1) == 0) {
                    found++;
                    found_using[f_ptr_idx]++;
                }

                free(str);
            }
        }
    }

    destroy_puzzle(puzzle);
    fclose(fp);

    fprintf(stdout, "Day 4_1: Puzzle contains XMAS %d times\n", found);
}

bool needle_part2_matches_forward_or_reverse(char* str) {
    if (strcmp(str, needle_part_2) == 0) {
        return true;
    }

    // Reverse the string
    size_t str_len = strlen(str);
    char* reversed = malloc(sizeof(char) * str_len);
    for (size_t i = 0; i < str_len; i++) {
        reversed[i] = str[str_len - i - 1];
    }

    if (strcmp(reversed, needle_part_2) == 0) {
        free(reversed);
        return true;
    }

    free(reversed);
    return false;
}

void solve_day4_2(const char *puzzle_input) {
    FILE *fp = fopen(puzzle_input, "r");
    if (fp == NULL) {
        fprintf(stderr, "Could not load puzzle input file: %s\n", puzzle_input);
        return;
    }

    puzzle_t *puzzle = load_puzzle(fp);

    int found = 0;
    const int size = 3;

    for (int x = 0; x < puzzle->num_lines; x++) {
        for (int y = 0; y < puzzle->line_length; y++) {
            char *xpart = get_diag_bottom_right(puzzle, x, y, size);
            if (!needle_part2_matches_forward_or_reverse(xpart)) {
                free(xpart);
                continue;
            }
            free(xpart);

            xpart = get_diag_top_right(puzzle, x + 2, y, size);
            if (needle_part2_matches_forward_or_reverse(xpart)) {
                found++;
            }
            free(xpart);
        }
    }

    destroy_puzzle(puzzle);
    fclose(fp);

    fprintf(stdout, "Day 4_1: Puzzle contains XMAS %d times\n", found);
}
