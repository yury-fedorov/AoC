#include "common.h"

TEST(AoC22, Day01) {
  const auto data = ReadData("01");
  std::vector<long> sums;
  auto sum = 0L;
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
  EXPECT_EQ(sum_highest_n(1), 70698);
  EXPECT_EQ(sum_highest_n(3), 206643);
}