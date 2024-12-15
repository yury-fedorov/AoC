#include "common-std.h"
#include "day22.h"
#include <cassert>
#include <iostream>

long day22_part2() {
  // day22::FinalPassword(6, 8, day22::Direction::kRight);
  return day22::Answer2("22");
}

int main(int argc, char **argv) {
  // too high : 158180
  std::cout << "Day 22 Part 2 (158180 is too high): " << day22_part2()
            << std::endl;
  return 0;
}