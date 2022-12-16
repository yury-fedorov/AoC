// #include "absl/strings/numbers.h"
#include "common.h"

TEST(AoC22, Day15) {
  const auto data = ReadData("15");
  /*
  std::vector<long> sums;
  long sum{0};
  for (const std::string &line : data) {
    if (long value{0}; absl::SimpleAtoi(line, &value)) {
      sum += value;
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
  */
}