#include <stdlib.h>
#include "libreports.h"
#include "libstringutils.h"
#include <stdio.h>
#include <string.h>

#define BAD_REPORT_PTR (-127)
#define REPORT_OK (-1)

report_t* create_report(const int size){
    report_t* report = malloc(sizeof(report_t));
    if (report == NULL) {
        return NULL;
    }

    report->size = size;
    report->reports = malloc(sizeof(int) * size);
    if (report->reports == NULL) {
        free(report);
        return NULL;
    }

    return report;
}

void destroy_report(report_t* report){
    if(report == NULL) {
        return;
    }

    if (report->reports != NULL) {
        free(report->reports);
    }
    free(report);
}

int find_first_faulty_report_element(const report_t* report) {
    if (report == NULL) {
        return BAD_REPORT_PTR;
    }

    bool all_increasing = false;
    for(int i = 0; i < report->size - 1; i++) {
        bool first_iteration = i == 0;
        if (first_iteration) {
            all_increasing = report->reports[1] > report->reports[0];
        }

        // Two adjacent elements must either increase or decrease
        if (report->reports[i] == report->reports[i+1]) {
            return i;
        }

        if (all_increasing && report->reports[i] > report->reports[i + 1]) {
            return i + 1;
        }

        if(!all_increasing && report->reports[i] < report->reports[i + 1]) {
            return i + 1;
        }

        int difference = all_increasing ?
                         report->reports[i+1] - report->reports[i] :
                         report->reports[i] - report->reports[i+1];

        // difference must be at least one and at most three
        if(difference > 3) {
            return i;
        }
    }

    return REPORT_OK;
}

void remove_report_element(report_t* report, const int index) {
    if (report == NULL ||  index > report->size - 1) {
        return;
    }

    void* remove_at = report->reports + index;
    const void* data_end = report->reports + report->size - 1;
    const size_t size = data_end - remove_at;
    memmove(remove_at, remove_at + 1 * sizeof(int), size);
    report->size--;
}

bool is_report_safe(const report_t* report, bool allow_single_faulty_level) {
    int result = find_first_faulty_report_element(report);
    if(result == REPORT_OK) {
        return true;
    }

    if(!allow_single_faulty_level) {
        return false;
    }

    // Brute force the solution by removing one element at a time
    for(int i = 0; i < report->size; i++) {
        report_t* report_copy = create_report(report->size);
        memcpy(report_copy->reports, report->reports, report->size * sizeof(int));
        remove_report_element(report_copy, i);
        if(find_first_faulty_report_element(report_copy) != REPORT_OK) {
            destroy_report(report_copy);
            continue;
        }

        destroy_report(report_copy);
        return true;
    }

    return false;
}

void read_reports(const char* puzzle_input, const int size, report_t** reports){
    FILE* fp = fopen(puzzle_input, "r");
    if (fp == NULL) {
        reports = NULL;
        fprintf(stderr, "Failed to open file: %s\n", puzzle_input);
        return;
    }

    ssize_t line_len;
    size_t line_buffer_size = 0;
    char* line = NULL;

    int reports_added = 0;
    while ((line_len = getline(&line, &line_buffer_size, fp)) != -1) {
        if (reports_added == size) {
            break;
        }

        int number_of_report_elements = str_contains_times(line, line_len, " ", 1) + 1;
        report_t* current_report = create_report(number_of_report_elements);

        int current_report_index = 0;
        char current_number[10];
        char* current_number_ptr = current_number;
        for(int i = 0; i <= line_len; i++) {
            if(line[i] == ' ' || line[i] == '\n' || i == line_len) {
                *current_number_ptr = 0;
                if(*current_number == 0) {
                    continue;
                }
                current_report->reports[current_report_index] = atoi(current_number);
                current_report_index++;
                current_number_ptr = current_number;
                continue;
            }

            *current_number_ptr = line[i];
            current_number_ptr++;
        }
        reports[reports_added] = current_report;
        reports_added++;
    }

    free(line);
    fclose(fp);
}