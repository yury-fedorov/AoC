#include "common.h"

namespace day25 {
  using Int = __int128; //long long;
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
  
  Snafu ToSnafu( Int value ) noexcept {
    Snafu result;
    constexpr double kRoot5 = 1.0 / 5.0
    auto start = pow( value, kRoot5 );
    return result;
  }
} 

// from Domodossola (Italy)
TEST(AoC22, Day25) {
  const auto data = ReadData("25");
  day25::Int sum = 0;
  for ( const auto & s : data ) {
    sum += day25::ToDecimal(s);
  }
  EXPECT_EQ(day25::ToDecimal("1=0"), 15);
  EXPECT_EQ(sum,day25::ToDecimal("4216113430113-")); // 4216113430113-
  EXPECT_EQ(day25::ToSnafu(3),"1="); 
  EXPECT_EQ(day25::ToSnafu(sum),"4216113430113-"); // what is right?
}
