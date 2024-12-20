#include "day1_1.h"
#include "day1_2.h"
#include "day2.h"
#include "day3.h"
#include "day4.h"
#include "day5.h"
#include "day6.h"
#include "day7.h"
#include "day8.h"

int main(void) {
    solve_day1_1("../puzzle_inputs/day1_input.txt");
    solve_day1_2("../puzzle_inputs/day1_input.txt");
    solve_day2_1("../puzzle_inputs/day2_input.txt");
    solve_day2_2("../puzzle_inputs/day2_input.txt");
    solve_day3("../puzzle_inputs/day3_input.txt", 1);
    solve_day3("../puzzle_inputs/day3_input.txt", 2);
    solve_day4_1("../puzzle_inputs/day4_input.txt");
    solve_day4_2("../puzzle_inputs/day4_input.txt");
    solve_day5("../puzzle_inputs/day5_input.txt", 1);
    solve_day5("../puzzle_inputs/day5_input.txt", 2);
    solve_day_6_1("../puzzle_inputs/day6_input.txt");
    solve_day_6_2("../puzzle_inputs/day6_input.txt");
    solve_day7("../puzzle_inputs/day7_input.txt",1);
    solve_day7("../puzzle_inputs/day7_input.txt",2);
    solve_day8_1("../puzzle_inputs/day8_input.txt");
    solve_day8_2("../puzzle_inputs/day8_input.txt");
}
