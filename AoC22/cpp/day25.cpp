#include "common.h"

namespace day25 {
using Int = int64_t;
using Snafu = std::string;
[[nodiscard]] constexpr Int ToDecimal(std::string_view snafu) noexcept {
  auto it = snafu.rbegin();
  const auto end = snafu.rend();
  Int sum{0};
  for (Int v = 1; it != end; v *= 5, it++) {
    const char c{*it};
    const int k = c == '=' ? -2 : c == '-' ? -1 : (c - '0');
    sum += k * v;
  }
  return sum;
}

constexpr inline bool IsUp(int high) noexcept {
  return high >= 3;  // 3 -> 1=
}

// from Voghera (Italy)
inline bool IsGood(Int value, int position) noexcept {
  const Int ref_value = std::pow(5, position);
  const Int ref_low = position >= 1 ? std::pow(5, position - 1) : 0;
  return (3 * ref_value > value) && (value <= (ref_value - (2 * ref_low)));
}

// from Zug (CH)
inline int Position(Int value) noexcept {
  int position = 0;
  for (; true; position++) {
    if (3 * std::pow(5, position) > value) return position;
  }
}

/*
Snafu ToSnafu(Int value) noexcept {
  Snafu result;
  auto position = Position(value);
  while (value != 0) {
    EXPECT_TRUE(IsGood(value, position)) << "Wrong position " << position;
    break;  // TODO - missing implementation
  }
  return result;
}
*/

Int Sum(std::string_view file) noexcept {
  const auto data = ReadData(file);
  day25::Int sum = 0;
  for (const auto &s : data) {
    sum += day25::ToDecimal(s);
  }
  return sum;
}

}  // namespace day25

// from Domodossola (Italy)
TEST(AoC22, Day25) {
  EXPECT_EQ(day25::Sum("25-sample"), 4890);
  const auto sum = day25::Sum("25");
  EXPECT_EQ(day25::ToDecimal("1=0"), 15);
  EXPECT_EQ(day25::ToDecimal("1=0"), 15);
  EXPECT_EQ(sum, 36671616971741LL);

  using Pair = std::pair<int, std::string_view>;
  constexpr std::array kTests = {
      Pair{3, "1="},          Pair{4, "1-"},
      Pair{5, "10"},          Pair{15, "1=0"},
      Pair{20, "1-0"},        Pair{2022, "1=11-2"},
      Pair{12345, "1-0---0"}, Pair{314159265, "1121-1110-1=0"}};
  for (const auto &[decimal, snafu] : kTests) {
    // EXPECT_EQ(day25::ToSnafu(decimal), snafu)
    //     << "Bad encoding " << decimal << " to " << snafu;
    EXPECT_EQ(day25::ToDecimal(snafu), decimal)
        << "Bad encoding " << decimal << " to " << snafu;
  }
  // calculated using Google Sheet
  // TODO - finish the algorithm later?
  EXPECT_EQ(sum, day25::ToDecimal("20=02=120-=-2110-0=1"));
}
