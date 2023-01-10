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

[[nodiscard]] Map Iteration(const Map &start, int order) noexcept {
  absl::flat_hash_map<Point, std::vector<Point>> to_from;
  for (const auto &elf : start) {
    const auto elf_next = Next(start, elf, order);
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

// TODO: only for debug purposes  
std::string Print(const Map &map) {
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

} // namespace day23

TEST(AoC22, Day23) {
  const auto initial_map = day23::ReadMap("23-sample2");
  day23::Map map = initial_map;
  int order = 0;
  for (int i = 0; i < 10; i++) {
    const auto out0 = day23::Print(map);
    map = day23::Iteration(map, order);
    const auto out1 = day23::Print(map);
    order = (order + 1) % 4;
  }
  // TODO - debug -- EXPECT_EQ( day23::CountEmpty(map), -1);
}
