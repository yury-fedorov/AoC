#include "absl/container/flat_hash_map.h"
#include "absl/strings/numbers.h"
#include "common.h"

namespace day08 {
bool is_border(int x, int y, int max_x, int max_y) {
  return x == 0 || y == 0 || (x == max_x - 1) || (y == max_y - 1);
};
} // namespace day08

TEST(AoC22, Day08) {
  const auto data = ReadData("08-sample");
  using Point = std::pair<int, int>;
  absl::flat_hash_map<Point, int> map;
  int y = 0;
  int max_x = 0;
  for (const std::string &line : data) {
    const auto ll = line.length();
    if (ll == 0)
      continue;
    max_x = ll;
    for (int x = 0; x < max_x; x++) {
      map[{x, y}] = static_cast<int>(line[x] - '0');
    }
    y++;
  }
  const int max_y = y;
  EXPECT_EQ(map.size(), max_x * max_y);
  const auto is_visible_outside = [&map, max_x, max_y](const Point &p) {
    // any tree on a border is visible
    const int x = p.first;
    const int y = p.second;
    if (day08::is_border(x, y, max_x, max_y))
      return true;
    // now only internal trees to consider, so we need to look from 4 sides
    const int h = map[p];
    const std::array dxy = {std::make_pair(0, 1), std::make_pair(0, -1),
                            std::make_pair(1, 0), std::make_pair(-1, 0)};
    for (const auto &[dx, dy] : dxy) {
      auto xi = x;
      auto yi = y;
      const auto hi = h;
      // if the height goes down (it is still visible)
      bool is_visible_in_this_direction = true;
      while (true) {
        xi += dx;
        yi += dy;
        const auto hj = map[{xi, yi}];
        if (hj >= hi) {
          is_visible_in_this_direction = false;
          break;
        }
        // hi = hj;
        if (day08::is_border(xi, yi, max_x, max_y))
          break;
      }
      if (is_visible_in_this_direction)
        return true;
    }
    return false;
  };

    const auto get_scenic_score = [&map, max_x, max_y](const Point &p) -> long long {
      // any tree on a border is visible
      const int x = p.first;
      const int y = p.second;
      if ( day08::is_border(x, y, max_x, max_y) ) return 0;
      const int h = map[p];
      const std::array dxy = {std::make_pair(0, 1), std::make_pair(0, -1),
                              std::make_pair(1, 0), std::make_pair(-1, 0)};
      long long score {1};
      for (const auto &[dx, dy] : dxy) {
        auto xi = x;
        auto yi = y;
        long viewing_distance {0};
        while (true) {
          xi += dx;
          yi += dy;
          // if ( xi < 0 || yi < 0 || xi >= max_x || yi >= max_y ) break;
          const auto hi = map[{xi, yi}];
          if (hi <= h) {
            viewing_distance++;
          }
          if ( hi >= h || day08::is_border(xi, yi, max_x, max_y))
            break;
        }
        score *= viewing_distance;
      }
      return score;
    };


  long answer1{0};
  long long answer2{0};
  for (const auto &[p, _] : map) {
    answer1 += is_visible_outside(p) ? 1 : 0;
    answer2 = std::max(answer2, get_scenic_score(p)); // 126 - too low, 361920 -- too low
  }
  // EXPECT_EQ(answer1, 1647);
  EXPECT_EQ(answer2, 0);
}