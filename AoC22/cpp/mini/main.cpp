#include "common-std.h"
#include "day19.h"
#include "day22.h"
#include <cassert>
#include <chrono> // timer
#include <iostream>

long day19_part1() {
  using std::chrono::duration_cast;
  using std::chrono::high_resolution_clock;
  using std::chrono::seconds;

  const auto blueprints = day19::ReadBlueprints("19");
  assert(blueprints.size() == 30);

  const day19::IdGeodesList test_list = {{1, 9}, {2, 12}};
  assert(day19::QualityLevel(test_list) == 33);

  // 733 seconds on local PC
  const auto t1 = high_resolution_clock::now();
  // answer 1
  day19::IdGeodesList list;
  for (const auto &b : blueprints) {
    list[b.id] = day19::LargestGeodes1(b);
  }
  const auto result = day19::QualityLevel(list);
  assert(result == 600);
  const auto t2 = high_resolution_clock::now();
  /* Getting number of milliseconds as an integer. */
  const auto sec_int = duration_cast<seconds>(t2 - t1);
  std::cout << "Timing in seconds: " << sec_int << std::endl; // 4 seconds!
  return result;
}

long day22_part2() {
  // day22::FinalPassword(6, 8, day22::Direction::kRight);
  return day22::Answer2("22");
}

int main(int argc, char **argv) {
  std::cout << "Day 19 Part 1 (600 approx 733 sec): " << day19_part1()
            << std::endl;
  if (IsFastOnly()) {
    std::cout << "Also Day 22 is not solved yet!" << std::endl;
  } else {
    std::cout << "Day 22 Part 2: " << day22_part2() << std::endl;
  }
  return 0;
}