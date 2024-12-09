#include "day1_1.h"
#include "day1_2.h"
#include "day2.h"
#include "day3.h"

int main(void) {
    solve_day1_1("../puzzle_inputs/day1_input.txt");
    solve_day1_2("../puzzle_inputs/day1_input.txt");
    solve_day2_1("../puzzle_inputs/day2_input.txt");
    solve_day2_2("../puzzle_inputs/day2_input.txt");
    solve_day3("../puzzle_inputs/day3_input.txt", 1);
    solve_day3("../puzzle_inputs/day3_input.txt", 2);
}
