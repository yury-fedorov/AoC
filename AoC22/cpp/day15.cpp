#include "absl/container/flat_hash_set.h" // not efficient
#include "common.h"
#include <re2/re2.h>

namespace day15 {
using Point = std::pair<int, int>;
using SensorBeacon = std::pair<Point, Point>;
using Range = std::pair<int, int>;
using Set = absl::flat_hash_set<int>;

[[nodiscard]] std::vector<SensorBeacon> Read(std::string_view file) noexcept {
  std::vector<SensorBeacon> result;
  const auto data = ReadData(file);
  // Sensor at x=2, y=18: closest beacon is at x=-2, y=15
  re2::RE2 re("Sensor at x=([-\\d]+), y=([-\\d]+): closest beacon is at "
              "x=([-\\d]+), y=([-\\d]+)");
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

[[nodiscard]] inline int Distance(const Point &a, const Point &b) noexcept {
  return abs(a.first - b.first) + abs(a.second - b.second);
}

constexpr int k_y = 2000000;

[[nodiscard]] std::optional<Range> GetRange(const SensorBeacon &sb,
                                            int y) noexcept {
  const auto [sensor, beacon] = sb;
  const auto distance = Distance(sensor, beacon);
  const auto [sx, sy] = sensor;
  const auto dy = abs(sy - y);        // how distant sensor is from y
  const auto range_x = distance - dy; // how much freedom we have on x
  if (range_x < 0)
    return std::optional<Range>();
  return Range{sx - range_x, sx + range_x};
}

[[nodiscard]] size_t Answer1(std::string_view file, int y) noexcept {
  const auto data = Read(file);
  day15::Set set;
  for (const auto &sb : data) {
    const auto opt_range = GetRange(sb, y);
    if (!opt_range.has_value())
      continue;
    const auto [min, max] = opt_range.value();
    for (int x = min; x <= max; x++) {
      set.insert(x);
    }
  }
  for (const auto &sb : data) {
    const auto &beacon = sb.second;
    const auto [bx, by] = beacon;
    if (by == y) {
      auto i = set.find(bx);
      if (i != set.end())
        set.erase(i);
    }
  }
  return set.size();
}

/* TODO part2
using RangeList = std::vector<Range>;
constexpr int k_lowest = 0;
constexpr int k_largest = 4000000;
[[nodiscard]] int Answer2(std::string_view file, int y) noexcept {
  const auto data = Read(file);
  for (int i = k_lowest; i <= k_largest;) {
    for (const auto &sb : data) {
      const auto opt_range = GetRange(sb, y);
      if (!opt_range.has_value())
        continue;
      const auto [min, max] = opt_range.value();
      if (i >= min && i < max) {
        i = max;
        break;
      }
    }
    i++;
  }
  return i;
}
}
*/

[[nodiscard]] inline long long TurningFrequency(const Point &p) noexcept {
  const auto [x, y] = p;
  return (x * 4000000LL) + y;
}

} // namespace day15

TEST(AoC22, Day15) {
  EXPECT_EQ(day15::Answer1("15-sample", 10), 26);
  // takes 14 seconds EXPECT_EQ(day15::Answer1("15", day15::k_y), 5127797);
  EXPECT_EQ(day15::TurningFrequency({14, 11}), 56000011);
}