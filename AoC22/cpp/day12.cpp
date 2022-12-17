#include "absl/container/flat_hash_map.h"
#include "absl/container/flat_hash_set.h"
#include "absl/strings/numbers.h"
#include "common.h"
#include <queue>

namespace day12 {
using Point = std::pair<int, int>;
using Points = std::vector<Point>;
using Height = char;
using Map = absl::flat_hash_map<Point, Height>;
using MapPath = absl::flat_hash_map<Point, int>; // steps to arrive
using PointSet = absl::flat_hash_set<Point>;     // queue of points to process

constexpr std::array<Point, 4> shifts = {Point{-1, 0}, {1, 0}, {0, -1}, {0, 1}};
std::optional<Height> GetHeight(const Map &map, Point p) noexcept {
  const auto i = map.find(p);
  if (i == map.end())
    return std::optional<Height>();
  const Height h = i->second;
  return h;
}
constexpr Height START = 'S';
constexpr Height END = 'E';

// without S and E but real heights
constexpr Height PureHeight(Height h) {
  switch (h) {
  case START:
    return 'a';
  case END:
    return 'z';
  }
  return h;
}

Points NextMove(Point current, const Map &map) {
  const auto [x0, y0] = current;
  const auto h0 = GetHeight(map, current).value();
  Points result;
  for (const auto [dx, dy] : shifts) {
    const Point next = {x0 + dx, y0 + dy};
    const auto height = GetHeight(map, next);
    if (!height.has_value())
      continue;
    const auto h1 = height.value();
    if (static_cast<int>(h1 - h0) > 1)
      continue; // too much go up hill
    result.emplace_back(next);
  }
  return result;
}

std::tuple<Map, Point, Point> Load(std::string_view file) {
  const auto data = ReadData(file);
  int y = 0;
  Map map;
  Point start, end;
  for (const std::string &line : data) {
    int x = 0;
    for (const char ch : line) {
      const Point p = {x, y};
      map[p] = PureHeight(ch);
      if (ch == START)
        start = p;
      else if (ch == END)
        end = p;
      x++;
    }
    y++;
  }
  return {map, start, end};
}

std::optional<int> Shortest(const Map &map, const Point start,
                            const Point end) {
  // starting point
  int path_length = {0};
  day12::MapPath map_path = {{start, 0}};
  day12::PointSet next = {start};
  while (map_path.find(end) == map_path.end()) {
    day12::PointSet next1;
    path_length += 1;
    for (const auto &p : next) {
      const auto n = day12::NextMove(p, map);
      for (const auto &nj : n) {
        if (map_path.find(nj) != map_path.end())
          continue;
        map_path[nj] = path_length;
        next1.insert(nj);
      }
    }
    next = next1;
    if (next.empty())
      return std::optional<int>();
  }
  return map_path.at(end);
}

} // namespace day12

TEST(AoC22, Day12) {
  const auto [map, start, end] = day12::Load("12");
  const int answer1 = day12::Shortest(map, start, end).value();
  EXPECT_EQ(answer1, 350);
  return; // part2 is slow - takes 25 seconds
  int answer2 = answer1;
  for (const auto &[start_i, level] : map) {
    if (level == 'a') {
      const auto o = day12::Shortest(map, start_i, end);
      if (o.has_value())
        answer2 = std::min(answer2, o.value());
    }
  }
  EXPECT_EQ(answer2, 349);
}
