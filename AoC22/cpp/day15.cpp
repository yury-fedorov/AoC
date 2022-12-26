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

// Intersection operations
[[nodiscard]] constexpr inline bool IsIntersection(Range a, Range b) noexcept {
  if (a.first > b.first)
    std::swap(a, b); // now a.first <= b.first
   // [a.first, a.second]
   //      [b.first
  return a.second >= b.first;
}

[[nodiscard]] std::optional<Range> Intersection(Range a, Range b) noexcept {
  if (!IsIntersection(a, b))
    return std::nullopt;
  return Range{std::min(a.first, b.first), std::max(a.second, b.second)};
}

void Sort(std::vector<Range> &ranges) noexcept {
  r::sort(ranges,
          [](const Range &a, const Range &b) { return a.first < b.first; });
}

void AddRange(std::vector<Range> &ranges, Range r) noexcept {
  Sort(ranges);
  for (int i = 0; i < ranges.size(); i++) {
    const auto opt_range = Intersection(ranges.at(i), r);
    if (opt_range.has_value()) {
      ranges.at(i) = std::move(opt_range.value());
      const auto i1 = i + 1;
      if (i1 < ranges.size()) {
        const auto opt_range1 = Intersection(ranges.at(i), ranges.at(i1));
        if (opt_range1.has_value()) {
          ranges.at(i) = opt_range1.value();
          ranges.erase(ranges.begin() + i1);
        }
      }
      return;
    }
  }
}

[[nodiscard]] constexpr inline bool IsInside(Range r, int value) noexcept {
  return r.first <= value && r.second >= value;
}

[[nodiscard]] constexpr inline bool IsValid(Range r) noexcept {
  return r.first <= r.second;
}

void RemovePoint(std::vector<Range> &ranges, int value) noexcept {
  for (int i = 0; i < ranges.size(); i++) {
    const Range r = ranges.at(i);
    if (IsInside(r, value)) {
      const Range r1{r.first, value - 1};
      const Range r2{value + 1, r.second};
      ranges.erase(ranges.begin() + i);
      if (IsValid(r1))
        ranges.emplace_back(std::move(r1));
      if (IsValid(r2))
        ranges.emplace_back(std::move(r2));
      Sort(ranges);
      return;
    }
  }
}
/*
[[nodiscard]] Set Minus( Range ab, const std::vector<Range> &ranges ) noexcept {
    Set result;
    for ( const auto & r : ranges ) {
    }
    return result;
}
*/

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

[[nodiscard]] std::vector<Range> GetSlice(const std::vector<SensorBeacon> &data,
                                         int y) noexcept {
  std::vector<Range> result;
  for (const auto &sb : data) {
    const auto opt_range = GetRange(sb, y);
    if (!opt_range.has_value())
      continue;
    AddRange(result, opt_range.value());
    const auto &beacon = sb.second;
    const auto [bx, _] = beacon;
    RemovePoint(result, bx);
  }
  return result;
}

// part2
using RangeList = std::vector<Range>;
constexpr int k_lowest = 0;
constexpr int k_largest = 4000000;
[[nodiscard]] int Answer2(std::string_view file) noexcept {
  const auto data = Read(file);
  for (int y = k_lowest; y <= k_largest; y++) {
    const auto ranges = GetSlice(data, y);
    if (ranges.size() > 1)
      return y;
  }
  return -1;
}

[[nodiscard]] inline long long TurningFrequency(const Point &p) noexcept {
  const auto [x, y] = p;
  return (x * 4000000) + y;
}

} // namespace day15

TEST(AoC22, Day15) {
  EXPECT_EQ(day15::Answer1("15-sample", 10), 26);
  EXPECT_EQ(day15::TurningFrequency({14, 11}), 56000011);
  // TODO - fixme -- EXPECT_EQ(day15::Answer2("15"), -2);
  if (IsFastOnly()) {
    return;
  }
  // takes 14 seconds
  EXPECT_EQ(day15::Answer1("15", day15::k_y), 5127797);
}
