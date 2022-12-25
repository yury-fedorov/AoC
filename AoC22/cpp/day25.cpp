#include "common.h"

namespace day25 {
  using Int = long long;
  using Snafu = std::string;
  Int ToDecimal(std::string_view snafu) noexcept {
    auto it = snafu.rbegin();
    const auto end = snafu.rend();
    Int sum {0};
    for (int v=1; it != end; v*=5, it++) {
      const char c { *it };
      const int k = c == '=' ? -2 : c == '-' ? -1 : ( c - '0' );
      sum += k * v;
    }
    return sum;
  }
} 

TEST(AoC22, Day25) {
  const auto data = ReadData("25"); 
  EXPECT_EQ(day25::ToDecimal("1=0"), 15);
}
