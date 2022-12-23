#include "common.h"

TEST(AoC22, Day04) {
  const auto data = ReadData("04");
  long answer1 = 0;
  long answer2 = 0;
  int a, b, x, y;
  for (const std::string &line : data) {
    // TODO - switch to re2?
    sscanf(line.c_str(), "%d-%d,%d-%d", &a, &b, &x, &y);
    if (a > x) {
      std::swap(a, x);
      std::swap(b, y);
    }
    // now a <= x
    // we search for: a...x...y...b
    answer1 += (y <= b) || (a == x) ? 1 : 0;
    answer2 += (b >= x) ? 1 : 0;
  }
  EXPECT_EQ(answer1, 582);
  EXPECT_EQ(answer2, 893);
}
