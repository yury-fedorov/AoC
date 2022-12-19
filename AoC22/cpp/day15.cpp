#include "common.h"
#include <re2/re2.h>
// Sensor at x=2, y=18: closest beacon is at x=-2, y=15

namespace day15 {
using Point = std::pair<int, int>;
using SensorBeacon = std::pair<Point, Point>;

[[nodiscard]] std::vector<SensorBeacon> Read(std::string_view file) noexcept {
  // Sensor at x=2, y=18: closest beacon is at x=-2, y=15
  std::vector<SensorBeacon> result;
  const auto data = ReadData(file);
  re2::RE2 re(
      "Sensor at x=(\\d+), y=(\\d+): closest beacon is at x=(\\d+), y=(\\d+)");
  for (const std::string &line : data) {
    if (line.empty())
      continue;
    re2::StringPiece input(line);
    int sx, sy, bx, by;
    if (re2::RE2::FullMatch(input, re, &sx, &sy, &bx, &by)) {
      result.emplace_back(SensorBeacon{{sx, sy}, {bx, by}});
    } else {
      EXPECT_TRUE(false) << line;
    }
  }
  return result;
}
} // namespace day15

TEST(AoC22, Day15) {
  const auto data = day15::Read("15");
  EXPECT_EQ(data.size(), 0);
}