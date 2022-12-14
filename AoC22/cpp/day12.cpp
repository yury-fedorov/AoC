#include "absl/container/flat_hash_map.h"
#include "absl/container/flat_hash_set.h"
#include "absl/strings/numbers.h"
#include "common.h"

namespace day12 {
using Point = std::pair<int, int>;
using Points = std::vector<Point>;
using Height = char;
using Map = absl::flat_hash_map<Point, Height>;
using MapPath = absl::flat_hash_map<Point, int>; // steps to arrive
using NextQueue =
    absl::flat_hash_set<Point>; // queue of points to process - TODO

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
    if (h1 == START)
      continue; // do not return
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
      map[p] = ch;
      if (ch == START)
        start = p;
      else if (ch == END )
        end = p;
      x++;
    }
    y++;
  }
  return {map, start, end};
}
} // namespace day12

TEST(AoC22, Day12) {
  const auto [map, start, end] = day12::Load("12");
  // starting poin
  std::vector<day12::Point> queue = {start};
  while (!queue.empty()) {
  	const auto p = queue.back();
  	queue.pop_back();
  }
  /*
  const auto sum_highest_n = [&sums](size_t n) {
    return std::accumulate(sums.rbegin(), sums.rbegin() + n, 0);
  };
  EXPECT_EQ(sum_highest_n(1), 70698);
  // EXPECT_EQ(sum_highest_n(3), 206643);
  */
}
