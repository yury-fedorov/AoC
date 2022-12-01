#include "common.h"

using namespace std;

namespace day01 {}

TEST_CASE("Day01", "[01]") {
  using namespace day01;
  const auto data = ReadData("1");
  std::vector<long> sums;
  auto sum = 0;
  for (const std::string &line : data) {
    if (line.length()) {
      sum += std::stol(line);
    } else {
      sums.insert(std::ranges::upper_bound(sums, sum), sum);
      sum = 0;
    }
  }
  const auto sum_highest_n = [&sums](size_t n) {
    return std::accumulate(sums.rbegin(), sums.rbegin() + n, 0);
  };
  SECTION("01-1") { REQUIRE(sum_highest_n(1) == 70698); }
  SECTION("01-2") { REQUIRE(sum_highest_n(3) == 206643); }
}