#include <stdbool.h>

#ifndef AOC_2024_LIBREPORTS_H
#define AOC_2024_LIBREPORTS_H
typedef struct {
    int size;
    int* reports;
} report_t;

report_t* create_report(int size);

void destroy_report(report_t* report);

bool is_report_safe(const report_t* report, bool allow_single_faulty_level);

void read_reports(const char* puzzle_input, int size, report_t** reports);

#endif //AOC_2024_LIBREPORTS_H
