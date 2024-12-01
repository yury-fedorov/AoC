#include "absl/container/flat_hash_set.h"
#include "common.h"
#include "re2/re2.h"

namespace day18 {
using Point3 = std::tuple<int, int, int>;
using PointSet = absl::flat_hash_set<Point3>;

constexpr std::array<Point3, 6> kShifts = {
    Point3{-1, 0, 0}, {1, 0, 0}, {0, -1, 0}, {0, 1, 0}, {0, 0, -1}, {0, 0, 1}};

[[nodiscard]] Point3 Limit(const PointSet &list,
                           std::function<int(int, int)> limit) noexcept {
  auto [a, b, c] = *list.begin();
  for (const auto [x, y, z] : list) {
    a = limit(a, x);
    b = limit(b, y);
    c = limit(c, z);
  }
  return {a, b, c};
}

[[nodiscard]] bool Contains(const PointSet &space, const Point3 &p) noexcept {
  return space.find(p) != space.end();
}

[[nodiscard]] std::pair<int, int> Solution(std::string_view file) noexcept {
  const auto data = ReadData(file);
  PointSet positions;
  re2::RE2 re("(\\d+),(\\d+),(\\d+)");
  for (const std::string &line : data) {
    int x, y, z;
    re2::StringPiece input(line);
    if (re2::RE2::FullMatch(input, re, &x, &y, &z)) {
      positions.insert({x, y, z});
    } else
      EXPECT_TRUE(false) << line;
  }

  int answer1{0};
  for (const auto [x, y, z] : positions) {
    for (const auto [dx, dy, dz] : kShifts) {
      const Point3 p = {x + dx, y + dy, z + dz};
      answer1 += Contains(positions, p) ? 0 : 1;
    }
  }

  // part 2
  const auto a = Limit(positions, [](int a, int b) { return std::min(a, b); });
  const auto b = Limit(positions, [](int a, int b) { return std::max(a, b); });
  const auto [ax_, ay_, az_] = a;
  const auto [bx_, by_, bz_] = b;
  const auto ax = ax_ - 1, ay = ay_ - 1, az = az_ - 1;
  const auto bx = bx_ + 1, by = by_ + 1, bz = bz_ + 1;

  PointSet empty;
  for (int x = ax; x <= bx; x++) {
    for (int y = ay; y <= by; y++) {
      for (int z = az; z <= bz; z++) {
        const Point3 p{x, y, z};
        if ((x == ax || x == bx) && (y == ay || y == by) &&
            (z == az || z == bz)) {
          empty.insert(p);
        }
      }
    }
  }
  EXPECT_FALSE(empty.empty());

  PointSet border;
  PointSet next = empty;
  PointSet visited;
  PointSet all_empty = empty;
  while (!next.empty()) {
    PointSet next1;
    for (const auto pn : next) {
      visited.insert(pn);
      const auto [x, y, z] = pn;
      for (const auto [dx, dy, dz] : kShifts) {
        const Point3 p = {x + dx, y + dy, z + dz};
        const auto [px, py, pz] = p;
        if (px < ax || px > bx || py < ay || py > by || pz < az || pz > bz) {
          // out of the box is for sure empty
          all_empty.insert(p);
          continue;
        }
        if (Contains(visited, p))
          continue;
        if (Contains(positions, p)) {
          // this position is inside lava
          border.insert(p);
          continue;
        } else {
          all_empty.insert(p);
        }
        next1.insert(p);
      }
    }
    next = next1;
  }

  int answer2 = 0;
  for (const auto &bp : border) {
    const auto [x, y, z] = bp;
    for (const auto [dx, dy, dz] : kShifts) {
      const Point3 p = {x + dx, y + dy, z + dz};
      answer2 += Contains(all_empty, p) ? 1 : 0;
    }
  }

  return {answer1, answer2};
}
} // namespace day18

TEST(AoC22, Day18) {
  const auto [t1, t2] = day18::Solution("18-sample");
  EXPECT_EQ(t1, 64);
  EXPECT_EQ(t2, 58);

  const auto [a1, a2] = day18::Solution("18");
  EXPECT_EQ(a1, 4456);
  EXPECT_EQ(a2, 2510);
}
