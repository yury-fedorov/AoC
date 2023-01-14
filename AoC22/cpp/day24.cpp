#include "absl/container/flat_hash_map.h"
#include "absl/container/flat_hash_set.h"
#include "common.h"
namespace day24 {

using Map = std::vector<std::string>;
using Point = std::pair<int, int>;
using Queue = absl::flat_hash_set<Point>;
struct Blizzard {
  Point start;
  Point shift; // dx, dy
  Point last;  // from last to first
  Point first;
};

const absl::flat_hash_map<char, Point> kDirectionMap = {
    {'<', {-1, 0}}, {'^', {0, -1}}, {'v', {0, 1}}, {'>', {1, 0}}};

constexpr char kWall = '#';

[[nodiscard]] char At(const Map &map, Point at) noexcept {
  const auto [x, y] = at;
  if (x < 0 || y < 0 || y >= map.size())
    return kWall;
  const auto &line = map[y];
  if (x >= line.length())
    return kWall;
  return line[x];
}

[[nodiscard]] Point Shift(Point point, Point shift) noexcept {
  return {point.first + shift.first, point.second + shift.second};
}

[[nodiscard]] Point AtBorder(const Map &map, Point point,
                             Point shift) noexcept {
  while (true) {
    const Point next = Shift(point, shift);
    if (At(map, next) == kWall)
      break;
    point = next;
  }
  return point;
}

[[nodiscard]] Blizzard CreateBlizzard(const Map &map, Point start) noexcept {
  const char direction = At(map, start);
  const Point shift = kDirectionMap.at(direction);
  const Point negative_shift{-shift.first, -shift.second};
  const Point last = AtBorder(map, start, shift);
  const Point first = AtBorder(map, start, negative_shift);
  return {start, shift, last, first};
}

using Blizzards = std::vector<Blizzard>;
[[nodiscard]] Point Position(const Map &map, Point start, size_t t) noexcept {
  // TODO (direction, size of moving, last, first position)
  return start;
}

[[nodiscard]] size_t Distance(Point a, Point b) noexcept {
  const auto [ax, ay] = a;
  const auto [bx, by] = b;
  return abs(ax - bx) + abs(ay - by);
}

[[nodiscard]] constexpr int Index(std::string_view line) noexcept {
  return line.find('.');
}

[[nodiscard]] size_t Answer1(const Map &map) noexcept {
  // TODO
  Point start{Index(map[0]), 0};
  const int y_max = map.size() - 1;
  Point finish{Index(map[y_max]), y_max};
  const auto till_finish = [&finish](const Point p) {
    return Distance(finish, p);
  };
  return till_finish(
      start); // 145 min, 152 too low, 160 too low (10%), 174 too low (20%)

  size_t t{0};
  Queue frontline = {start};
  while (true) {
    break;
    t++;
    Queue next;
    for (const Point &p0 : frontline) {
      // logic on current state the cell can be occupied by a blizzard
      // but it must be free on the next step
      // 5 options: up, down, right, left, stay
      Point p1;
      if (p1 == finish)
        break;
    }
    /*
    constexpr size_t kMaxDelay = 1; // we consider at max n cells delay to be in
    optimal const auto [min, max] = absl::c_minmax_element( next.begin(),
    next.end(),
    [&till_finish](auto a, auto b) {
      return till_finish(a) < till_finish(b);
    } );
    const auto min_remains = till_finish(*min);
    const auto max_remains = till_finish(*max);
    if ( max_remains - min_remains > kMaxDelay ) {
      // we need to remove all elements which are too dist
    }
    break; // TODO
    */
  }
  return t;
}

} // namespace day24

TEST(AoC22, Day24) {
  const day24::Map map = ReadData("24");
  EXPECT_EQ(day24::Answer1(map), 0);
}
