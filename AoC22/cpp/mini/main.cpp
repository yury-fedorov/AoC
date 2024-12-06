#include "common-std.h"
#include "day16.h"
#include "day19.h"
#include "day22.h"
#include <cassert>
#include <iostream>

long day16_part2() {
  const auto map = day16::ReadMap("16-sample");
  const long pressure = day16::Answer2(map);
  return pressure;
}

long day19_part1() {
  const day19::IdGeodesList test_list = {{1, 9}, {2, 12}};
  assert(day19::QualityLevel(test_list) == 33);
  if (IsFastOnly())
    return 0;
  // takes several minutes to answer even to 1, even for the sample
  const auto blueprints = day19::ReadBlueprints("19-sample");
  day19::IdGeodesList list;
  for (const auto &b : blueprints) {
    list[b.id] = day19::LargestGeodes1(b);
  }
  return day19::QualityLevel(list);
}

long day22_part2() {
  // day22::FinalPassword(6, 8, day22::Direction::kRight);
  return day22::Answer2("22");
}

int main(int argc, char **argv) {
  std::cout << "Day 16 Part 2 Sample (1707): " << day16_part2() << std::endl;
  if (IsFastOnly()) {
    std::cout << "Also Day 19 and Day 22 are not solved yet!" << std::endl;
  } else {
    std::cout << "Day 19 Part 1: " << day19_part1() << std::endl;
    std::cout << "Day 22 Part 2: " << day22_part2() << std::endl;
  }
  return 0;
}