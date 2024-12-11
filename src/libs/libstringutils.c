#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "libstringutils.h"

#include <ctype.h>


int str_count_char(const char* line, const char c) {
    int count = 0;
    for (int i = 0; line[i] != '\0'; i++) {
        if (line[i] == c) {
            count++;
        }
    }

    return count;
}

char *trim_space(char *str) {
    // Skip leading whitespace
    while (isspace(*str)) {
        str++;
    }

    // If all spaces or empty, return an empty string
    if (*str == '\0') {
        return str;
    }

    // Find the end of the string
    char *end = str + strlen(str) - 1;

    // Remove trailing whitespace
    while (end > str && isspace(*end)) {
        end--;
    }

    // Write new null terminator
    *(end + 1) = '\0';

    return str;
}

str_lines_t *str_read_lines(const char *file) {
    FILE *fp = fopen(file, "r");
    if (fp == NULL) {
        fprintf(stderr, "Failed to open file %s\n", file);
        return NULL;
    }

    size_t line_buffer_size = 5;
    char *line_buffer = calloc(line_buffer_size, sizeof(char));
    if (line_buffer == NULL) {
        fprintf(stderr, "Out of memory!\n");
        abort();
    }

    size_t lines_read = 0;
    size_t lines_buffer_size = 5;
    char **lines_buffer = calloc(lines_buffer_size, sizeof(char *));
    size_t *line_len_buffer = calloc(lines_buffer_size, sizeof(size_t));
    if (lines_buffer == NULL ||line_len_buffer == NULL) {
        fprintf(stderr, "Out of memory!\n");
        abort();
    }



    size_t line_len = 0;
    while ((line_len = getline(&line_buffer, &line_buffer_size, fp)) != -1) {
        if (lines_read >= lines_buffer_size) {
            lines_buffer_size *= 2;
            lines_buffer = realloc(lines_buffer, sizeof(char *) * line_buffer_size);
            line_len_buffer = realloc(line_len_buffer, sizeof(size_t) * line_buffer_size);
            if (lines_buffer == NULL || line_len_buffer == NULL) {
                fprintf(stderr, "Out of memory!\n");
                abort();
            }
        }

        char *trimmed = trim_space(line_buffer);
        size_t trimmed_line_len = strlen(trimmed);
        lines_buffer[lines_read] = strdup(trimmed);
        line_len_buffer[lines_read] = trimmed_line_len;
        lines_read++;
    }
    fclose(fp);

    str_lines_t *result = calloc(1, sizeof(str_lines_t));
    result->line_len = line_len_buffer;
    result->lines = lines_buffer;
    result->num_lines = lines_read;
    free(line_buffer);

    return result;
}

void str_destroy_lines(str_lines_t *lines) {
    if (lines == NULL) {
        return;
    }

    if (lines->lines != NULL) {
        for (size_t i = 0; i < lines->num_lines; i++) {
            free(lines->lines[i]);
        }
        free(lines->lines);
    }

    if (lines->line_len != NULL) {
        free(lines->line_len);
    }

    free(lines);
}

int str_contains_times(const char* haystack, const size_t haystack_size, const char* needle, const size_t needle_size) {
    int count = 0;

    for(int i = 0; i < haystack_size; i++) {
        if (i + needle_size > haystack_size) {
            break;
        }

        bool found_char = false;
        for(int y = 0; y < needle_size; y++) {
            found_char = haystack[i] == needle[y];
            if(!found_char) {
                break;
            }
        }

        if(found_char) {
            count++;
        }
    }

    return count;
}