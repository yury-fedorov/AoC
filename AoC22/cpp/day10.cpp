#include "common.h"

namespace day10 {
using ValueInTime = std::vector<long>;
ValueInTime Run(const std::vector<std::string> &data) {
  ValueInTime x_in_time;
  long x{1};
  int circle{0};
  constexpr auto NOOP = "noop";
  for (const std::string &line : data) {
    int circles;
    long x1 = {x};
    if (line.starts_with(NOOP)) {
      circles = 1;
    } else if (int arg{0}; EOF != sscanf(line.c_str(), "addx %d", &arg)) {
      circles = 2;
      x1 = x + arg;
    }
    for (int i = 0; i < circles; i++, circle++) {
      x_in_time.push_back(x);
    }
    x = x1;
  }
  return x_in_time;
}
} // namespace day10

TEST(AoC22, Day10) {
  const auto data = ReadData("10"); // ReadData("10-sample2");
  const auto x_in_time = day10::Run(data);
  long answer1{0};
  std::array key_circles = {20, 60, 100, 140, 180, 220};
  for (int i : key_circles) {
    answer1 += x_in_time[i - 1] * i;
  }
  EXPECT_EQ(answer1, 12840);
  std::string output;
  int crt_position{0};
  for (int sprite : x_in_time) {
    const char ch = abs((crt_position % 40) - sprite) <= 1 ? '#' : '.';
    output += ch;
    crt_position++;
    if ((crt_position) % 40 == 0)
      output += '\n';
  }
  EXPECT_TRUE(true) << output; // ZKJFBJFZ
}