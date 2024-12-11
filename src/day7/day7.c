#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "day7.h"
#include "libstringutils.h"

calibration_t **load_calibrations(const char *puzzle_input, size_t *result_size) {
    FILE *fp = fopen(puzzle_input, "r");
    if (fp == NULL) {
        fprintf(stderr, "Failed to load file: %s\n", puzzle_input);
        return NULL;
    }

    size_t line_buffer_size = 5;
    char *line_buffer = calloc(line_buffer_size, sizeof(char));
    if (line_buffer == NULL) {
        fclose(fp);
        return NULL;
    }

    size_t calibrations_size = 0;
    size_t calibrations_buffer_size = 5;
    calibration_t **calibrations = calloc(calibrations_buffer_size, sizeof(calibration_t*));

    if (calibrations == NULL) {
        fclose(fp);
        free(line_buffer);
        return NULL;
    }

    size_t line_len = 0;
    while ((line_len = getline(&line_buffer, &line_buffer_size, fp)) != -1) {
        if (line_buffer[line_len - 1] == '\n') {
            line_buffer[line_len - 1] = '\0';
        }

        calibration_t *calibration = calloc(1, sizeof(calibration_t));
        if (calibration == NULL) {
            for (size_t i = 0; i < calibrations_size; i++) {
                free(calibrations[i]->operands);
                free(calibrations[i]);
            }
            free(calibrations);
            fclose(fp);
            free(line_buffer);
            return NULL;
        }

        char *token = strdup(line_buffer);
        char *orig_token_ptr = token;

        char *str_result = strsep(&token, ":");
        char *strol_error;
        calibration->result = strtol(str_result, &strol_error, 10);

        if (*strol_error != '\0') {
            fprintf(stderr, "Warning: Input may be malformed: %s\n", line_buffer);
        }

        calibration->operands_size = count_char(line_buffer, ' ');
        calibration->operands = calloc(calibration->operands_size, sizeof(long));
        if (calibration->operands == NULL) {
            for (size_t i = calibrations_size; i > 0; i--) {
                free(calibrations[i]->operands);
                free(calibrations[i]);
            }
            free(calibrations);
            fclose(fp);
            free(line_buffer);
            return NULL;
        }

        token++; // Skip the space
        const char *operand = strsep(&token, " ");
        size_t operand_idx = 0;
        while (operand != NULL) {
            char *operand_error;
            calibration->operands[operand_idx] = strtol(operand, &operand_error, 10);
            if (*operand_error != '\0') {
                fprintf(stderr, "Warning: Input may be malformed: %s\n", line_buffer);
            }

            operand = strsep(&token, " ");
            operand_idx++;
        }
        free(orig_token_ptr);

        if (calibrations_buffer_size <= calibrations_size) {
            calibration_t **new_calibrations_buffer = realloc(calibrations, sizeof(calibration_t*) * calibrations_buffer_size * 2);
            if (new_calibrations_buffer == NULL) {
                for (size_t i = 0; i < calibrations_size; i++) {
                    free(calibrations[i]->operands);
                    free(calibrations[i]);
                }
                free(calibrations);
                fclose(fp);
                free(line_buffer);
                return NULL;
            }

            calibrations = new_calibrations_buffer;
            calibrations_buffer_size *= 2;
        }

        calibrations[calibrations_size] = calibration;
        calibrations_size++;
    }

    free(line_buffer);
    fclose(fp);
    *result_size = calibrations_size;
    return calibrations;
}

void destroy_calibrations(calibration_t **calibrations, size_t calibrations_size) {
    if (calibrations == NULL) {
        return;
    }

    for (size_t i = 0; i < calibrations_size; i++) {
        free(calibrations[i]->operands);
        free(calibrations[i]);
    }

    free(calibrations);
}

void debug_print_calibrations(calibration_t **calibrations, size_t calibrations_size) {
    for (size_t i = 0; i < calibrations_size; i++) {
        printf("Calibration %lu: %ld\n", i, calibrations[i]->result);
        for (size_t j = 0; j < calibrations[i]->operands_size; j++) {
            printf("Operand %lu: %ld\n", j, calibrations[i]->operands[j]);
        }
    }
}


bool test_calibration(const calibration_t *calibration, const size_t index, const long sum, const bool concat_enabled) { // NOLINT(*-no-recursion)
    if (index == calibration->operands_size) {
        return calibration->result == sum;
    }

    if (sum > calibration->result) {
        return false;
    }

    if (test_calibration(calibration, index + 1, sum * calibration->operands[index], concat_enabled)) {
        return true;
    }

    if (test_calibration(calibration, index + 1, sum + calibration->operands[index], concat_enabled)) {
        return true;
    }

    if (concat_enabled) {
        char str[8192];
        sprintf(str, "%lu%lu", sum, calibration->operands[index]);
        if (test_calibration(calibration, index + 1, strtol(str, NULL, 10), concat_enabled)) {
            return true;
        }
    }

    return false;
}

void solve_day7(const char *puzzle_input, const int part) {
    size_t calibrations_size = 0;
    calibration_t **calibrations = load_calibrations(puzzle_input, &calibrations_size);
    if (calibrations == NULL) {
        fprintf(stderr, "Failed to load calibrations!\n");
        return;
    }

    long result = 0;
    for (size_t i = 0; i < calibrations_size; i++) {
        calibration_t *calibration = calibrations[i];

        if (test_calibration(calibration, 1, calibration->operands[0], part == 2)) {
            result += calibration->result;
        }
    }

    destroy_calibrations(calibrations, calibrations_size);
    printf("Day 7_%d: Sum of all calibrations is %lu\n", part, result);
}
