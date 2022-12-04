#include "absl/strings/numbers.h"
#include "common.h"

TEST(AoC22, Day04) {
  const auto data = ReadData("04");
  // 67-84,66-87

  long answer1{0};
  int a, b, x, y;
  for (const std::string &line : data) {
    if (line.length() <7) continua;
    sscanf( line.c_str(), "%d-%d,%d-%d", &a, &b, &c, &d);
    if (a > x) {
       std::swap(a, x);
       std::swap(b, y);
    } 
    // now a <= x
    // we search for: a...x...y...b
    answer1 += (y<=b)? 1:0;
  }
  
  EXPECT_EQ(answer1, 0);
  // EXPECT_EQ(sum_highest_n(3), 206643);
}
