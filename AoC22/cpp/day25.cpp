#include "common.h"

namespace day25 {
using Int = __int128; // long long;
using Snafu = std::string;
Int ToDecimal(std::string_view snafu) noexcept {
  auto it = snafu.rbegin();
  const auto end = snafu.rend();
  Int sum{0};
  for (int v = 1; it != end; v *= 5, it++) {
    const char c{*it};
    const int k = c == '=' ? -2 : c == '-' ? -1 : (c - '0');
    sum += k * v;
  }
  return sum;
}

constexpr inline bool IsUp(int high) noexcept {
  return high >= 3; // 3 -> 1=
}

// from Voghera (Italy)
constexpr inline bool IsGood(Int value, int position) noexcept {
  const Int ref_value = std::pow(5, position);
  const Int ref_low = position >= 1 ? std::pow(5, position - 1) : 0;
  return (3 * ref_value > value) && (value <= (ref_value - (2 * ref_low)));
}

Snafu ToSnafu(Int value) noexcept {
  Snafu result;
  constexpr double kRoot5 = 1.0 / 5.0;
  auto start = std::pow(value, kRoot5);
  int position = std::ceil(start);
  if (!IsGood(value, position))
    position--;
  while (value != 0) {
    EXPECT_TRUE(IsGood(value, position)) << "Wrong position " << position;
    break; // todo
  }
  return result;
}
} // namespace day25

// from Domodossola (Italy)
TEST(AoC22, Day25) {
  const auto data = ReadData("25");
  day25::Int sum = 0;
  for (const auto &s : data) {
    sum += day25::ToDecimal(s);
  }
  EXPECT_EQ(day25::ToDecimal("1=0"), 15);
  EXPECT_EQ(sum, 5481165789LL);
  EXPECT_EQ(sum, day25::ToDecimal("4216113430113-")); // 4216113430113-
  EXPECT_EQ(day25::ToSnafu(3), "1=");
  EXPECT_EQ(day25::ToSnafu(sum), "4216113430113-"); // what is right?
}
