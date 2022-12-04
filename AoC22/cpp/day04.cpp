#include "absl/strings/numbers.h"
#include "common.h"
#include <set>

TEST(AoC22, Day04) {
  const auto data = ReadData("04");
  // 67-84,66-87

  long answer1{0};
  long answer2=0;
  int a, b, x, y;
  std::set<int> overlap;
  for (const std::string &line : data) {
    if (line.length() <7) continue;
    sscanf( line.c_str(), "%d-%d,%d-%d", &a, &b, &x, &y);
    if (a > x) {
       std::swap(a, x);
       std::swap(b, y);
    } 
    // now a <= x
    // we search for: a...x...y...b
    answer1 += (a<x)? ( (y<=b)? 1:0 ) : (1) ;
    for ( int i = x; i <= std::min(b, y); i++) 
        overlap.insert(i);
    answer2 += (b >= x) ? 1 : 0;
  }
  
  EXPECT_EQ(answer1, 582);
  EXPECT_EQ(answer2,0);
}
