#include "absl/container/flat_hash_map.h"
#include "absl/container/flat_hash_set.h"
#include "common.h"
namespace day24 {

using Map = std::vector<std::string>;
using Point = std::pair<int, int>;
using Queue = absl::flat_hash_set<Point>;
struct Blizzard {
  Point start;
  Point direction; // dx, dy
  Point last;      // from last to first
  Point first;
};

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

[[nodiscard]] size_t Answer1(const Map &map) noexcept {
  // TODO
  Point start;
  Point finish;
  const auto till_finish = [&finish](const Point p) {
    return Distance(finish, p);
  };

  size_t t{0};
  Queue frontline = {start};
  while (true) {
    break;
    t++;
    Queue next;
    for (const Point &p0 : frontline) {
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
