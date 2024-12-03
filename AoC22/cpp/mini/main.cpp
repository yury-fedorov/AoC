#include "day16.h"
#include <cassert>
#include <iostream>

long day16_part1() {
  const auto map = day16::ReadMap("16-sample");
  const long pressure = day16::Answer1(map);
  return pressure;
}

int main(int argc, char **argv) {
  std::cout << "Day 16 Part 1: " << day16_part1() << std::endl;
  return 0;
}