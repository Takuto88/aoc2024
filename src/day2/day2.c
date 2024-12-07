#include <stdio.h>
#include "day2.h"
#include "libreports.h"
#define NUM_OF_REPORTS 1000

int solve(char* puzzle_input, const int allowed_faulty_levels) {
    report_t* reports[NUM_OF_REPORTS];
    read_reports(puzzle_input, NUM_OF_REPORTS, reports);

    int safe_reports = 0;
    for(int i = 0; i < NUM_OF_REPORTS; i++) {
        report_t* report = reports[i];
        if(is_report_safe(report, allowed_faulty_levels)) {
            safe_reports++;
        }

        destroy_report(report);
    }
    return safe_reports;
}

void solve_day2_1(char* puzzle_input) {
    int safe_reports = solve(puzzle_input, false);
    fprintf(stdout, "Day 2_1: Number of safe reports: %d\n", safe_reports);
}

void solve_day2_2(char* puzzle_input) {
    int safe_reports = solve(puzzle_input, true);
    fprintf(stdout, "Day 2_2: Number of safe reports: %d\n", safe_reports);
}