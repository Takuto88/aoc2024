#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#define WINDOW_SIZE (7)
#define WINDOW_END (WINDOW_SIZE - 1)

typedef struct {
    char* token;
    size_t size;
} token_t;

bool mul_enabled = true;

/**
 * Updates the global variable mul_enabled if a do() or don't() token was encountered.
 *
 * @param window The window to check
 * @return The offset of the next item after the token or 0 if no token was encountered
 */
size_t update_mul_enabled(const char* window) {
    const token_t token_do = {
        .token = "do()",
        .size = 4,
    };

    const token_t token_dont = {
        .token = "don't()",
        .size = 7,
    };

    if (memcmp(window, token_do.token, sizeof(char) * token_do.size) == 0) {
        mul_enabled = true;
        return token_do.size;
    }

    if (memcmp(window, token_dont.token, sizeof(char) * token_dont.size) == 0) {
        mul_enabled = false;
        return token_dont.size;
    }

    return 0;
}

/**
 * Checks if the beginning of the window starts with a multiplication token.
 *
 * @param window The window to check
 * @return 0 if no multiplication token was encountered, otherwise the offset of the next item after the token
 */
size_t mul_token_encountered(const char* window) {
    const token_t token_mul = {
        .token = "mul(",
        .size = 4,
    };

    if (memcmp(window, token_mul.token, sizeof(char) * token_mul.size) != 0) {
        return 0;
    }

    return token_mul.size;
}

/**
 * Moves the window by shift_by and reads the next character from the file.
 *
 * @param window The window to move
 * @param fp The file pointer to read from
 * @param shift_by The amount of characters to shift the window by
 */
void move_window_and_read_next(char* window,  FILE* fp, size_t shift_by) {
    while (shift_by > 0) {
        char* beginning = &window[0];
        char* copy_from = &window[1];
        size_t copy_size = sizeof(char) * WINDOW_END;
        memmove(beginning, copy_from, copy_size);

        if (!feof(fp)) {
            window[WINDOW_END] = (char) fgetc(fp);
        }

        shift_by--;
    }
}

int test_for_number(FILE* fp, char* window) {
    int num_str_idx = 0;
    char num_str[10];
    while(num_str_idx < 10) {
        num_str[num_str_idx] = window[0];
        move_window_and_read_next(window, fp, 1);

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

void solve_day3(const char* puzzle_input, int part) {
    FILE* fp = fopen(puzzle_input, "r");
    if (fp == NULL) {
        fprintf(stderr, "Could not open file at: %s", puzzle_input);
        return;
    }

    int result = 0;
    int index = 0; // Counts only up to the window size
    char window[WINDOW_SIZE];

    while (!feof(fp)) {

        // Read up to 4 chars into a window
        if (index < WINDOW_SIZE) {
            window[index] = (char) fgetc(fp);
            index++;
            continue;
        }

        // Enable or disable multiplication (day3 part 2)
        size_t move_window_by = update_mul_enabled(window);
        if (move_window_by != 0) {
            move_window_and_read_next(window, fp, move_window_by);
            continue;
        }

        // Only for part 1 we need to consider do() and don't()
        if (!mul_enabled && part == 2) {
            move_window_and_read_next(window, fp, 1);
            continue;
        }

        // Check if our window contains "mul("
        move_window_by = mul_token_encountered(window);
        if(move_window_by != 0) {
            move_window_and_read_next(window, fp, move_window_by);
        } else {
            move_window_and_read_next(window, fp, 1);
            continue;
        }

        // We have a potential match - now we need to check if the next char is a first_number.
        int first_number = test_for_number(fp, window);
        // Check if we have a first_number AND if we have not encountered a ) yet
        if(first_number == 0 || window[0] == ')') {
            continue;
        }

        // We can test for a next number
        int second_number = test_for_number(fp, window);
        if(second_number != 0) {
            result += first_number * second_number;
        }
    }

    fclose(fp);
    mul_enabled = true;

    fprintf(stdout, "Day 3_%d: All instructions multiplied are: %d\n", part, result);
}
