#include "absl/container/flat_hash_map.h"
#include "absl/container/flat_hash_set.h"
#include "common.h"

namespace day23 {
using Point = std::pair<int, int>;
using Map = absl::flat_hash_set<Point>;
[[nodiscard]] Map ReadMap(std::string_view file) noexcept {
  const auto data = ReadData(file);
  Map map;
  int y = 0;
  for (const auto &line : data) {
    int x = 0;
    for (const char c : line) {
      if (c == '#')
        map.insert(Point{x, y});
      x++;
    }
    y++;
  }
  return map;
}

constexpr std::array kDirections = {Point{0, -1}, Point{0, 1}, Point{-1, 0},
                                    Point{1, 0}};

[[nodiscard]] bool IsEmpty(const Map &map, Point elf,
                           Point direction) noexcept {
  for (const auto i : {-1, 0, 1}) {
    const auto dx = direction.first == 0 ? i : direction.first;
    const auto dy = direction.second == 0 ? i : direction.second;
    const auto p = Point{elf.first + dx, elf.second + dy};
    if (map.find(p) != map.end())
      return false;
  }
  return true;
}

[[nodiscard]] Point Next(const Map &map, Point elf, int order) noexcept {
  for (int i = 0; i < 4; i++) {
    const auto index = (order + i) % 4;
    const Point &dir = kDirections.at(index);
    if (IsEmpty(map, elf, dir)) {
      return Point{elf.first + dir.first, elf.second + dir.second};
    }
  }
  return elf;
}

[[nodiscard]] bool IsEmptyAround(const Map &map, Point position) noexcept {
  const auto [x, y] = position;
  for (int dx = -1; dx <= 1; dx++) {
    for (int dy = -1; dy <= 1; dy++) {
      if (dx == 0 && dy == 0)
        continue;
      const auto x1 = x + dx;
      const auto y1 = y + dy;
      if (map.find({x1, y1}) != map.end())
        return false;
    }
  }
  return true;
}

[[nodiscard]] Map Iteration(const Map &start, int order) noexcept {
  absl::flat_hash_map<Point, std::vector<Point>> to_from;
  for (const auto &elf : start) {
    const auto elf_next =
        IsEmptyAround(start, elf) ? elf : Next(start, elf, order);
    to_from[elf_next].push_back(elf);
  }
  // now we cancel all movements where we have multiple from
  Map next_map;
  for (const auto &[to, from] : to_from) {
    if (from.size() == 1)
      next_map.insert(to);
    else {
      // we cannot move
      for (const auto &from_i : from) {
        next_map.insert(from_i);
      }
    }
  }
  return next_map;
}

[[nodiscard]] bool IsSame(const Map &a, const Map &b) noexcept {
  Map intersect;
  std::set_intersection(a.begin(), a.end(), b.begin(), b.end(),
                        std::inserter(intersect, intersect.begin()));
  return intersect.size() == a.size();
}

std::tuple<Point, Point> MinMax(const Map &map) noexcept {
  Point min = *map.begin();
  Point max = min;
  for (const auto &p : map) {
    max.first = std::max(max.first, p.first);
    max.second = std::max(max.second, p.second);
    min.first = std::min(min.first, p.first);
    min.second = std::min(min.second, p.second);
  }
  return {min, max};
}

[[nodiscard]] size_t CountEmpty(const Map &map) noexcept {
  const auto [min, max] = MinMax(map);
  return (max.first - min.first + 1) * (max.second - min.second + 1) -
         map.size();
}

[[nodiscard]] std::string Print(const Map &map) noexcept {
  const auto [min, max] = MinMax(map);
  std::string out;
  for (int y = min.second; y <= max.second; y++) {
    for (int x = min.first; x <= max.first; x++) {
      out += map.find({x, y}) == map.end() ? '.' : '#';
    }
    out += '\n';
  }
  return out;
}

[[nodiscard]] size_t Answer1(std::string_view file) noexcept {
  day23::Map map = day23::ReadMap(file);
  int order = 0;
  for (int i = 0; i < 10; i++) {
    map = day23::Iteration(map, order);
    order = (order + 1) % 4;
  }
  return day23::CountEmpty(map);
}

[[nodiscard]] size_t Answer2(std::string_view file) noexcept {
  day23::Map map = day23::ReadMap(file);
  int order = 0;
  size_t moves = 0;
  auto prev = day23::Print(map);
  while (true) {
    map = day23::Iteration(map, order);
    const auto next = day23::Print(map);
    moves++;
    if (prev == next)
      return moves;
    prev = next;
    order = (order + 1) % 4;
  }
}

} // namespace day23

TEST(AoC22, Day23) {
  EXPECT_EQ(day23::Answer1("23-sample2"), 110);
  EXPECT_EQ(day23::Answer1("23"), 4218);
  EXPECT_EQ(day23::Answer2("23-sample2"), 20);
  if (IsFastOnly())
    return; // slow - takes 83 seconds
  EXPECT_EQ(day23::Answer2("23"), 976);
}