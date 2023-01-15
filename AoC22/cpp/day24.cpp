#include "absl/container/flat_hash_map.h"
#include "absl/container/flat_hash_set.h"
#include "common.h"

namespace day24 {

using Map = std::vector<std::string>;
using Point = std::pair<int, int>;
using Points = absl::flat_hash_set<Point>;
struct Blizzard {
  Point start;
  Point shift; // dx, dy
  Point last;  // from last to first
  Point first;
  int distance;
};
using Blizzards = std::vector<Blizzard>;

const Point kUp{0, -1};
const Point kLeft{-1, 0};
const Point kDown{0, 1};
const Point kRight{1, 0};

const absl::flat_hash_map<char, Point> kDirectionMap = {
    {'<', kLeft}, {'^', kUp}, {'v', kDown}, {'>', kRight}};

const std::array kOptions = {kRight, kDown, Point{0, 0}, kUp, kLeft};

constexpr char kWall = '#';
constexpr char kGround = '.';

[[nodiscard]] char At(const Map &map, Point at) noexcept {
  const auto [x, y] = at;
  if (x < 0 || y < 0 || y >= map.size())
    return kWall;
  const auto &line = map[y];
  if (x >= line.length())
    return kWall;
  return line[x];
}

[[nodiscard]] Point Shift(Point point, Point shift, int n = 1) noexcept {
  return {point.first + (n * shift.first), point.second + (n * shift.second)};
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
  const Point backward_shift{-shift.first, -shift.second};
  const Point last = AtBorder(map, start, shift);
  const Point first = AtBorder(map, start, backward_shift);
  const auto z = shift.first != 0 ? [](Point p) { return p.first; }
                                  : [](Point p) { return p.second; };
  const int distance = abs(z(last) - z(first)) + 1;
  return {start, shift, last, first, distance};
}

[[nodiscard]] Blizzards InitBlizzards(const Map &map) noexcept {
  const int y_n = map.size();
  const auto is_blizzard = [](char c) { return c != kWall && c != kGround; };
  Blizzards blizzards;
  for (int y = 0; y < y_n; y++) {
    const auto &line = map.at(y);
    const int x_n = line.size();
    for (int x = 0; x <= x_n; x++) {
      const Point p{x, y};
      const char c = At(map, p);
      if (is_blizzard(c)) {
        blizzards.emplace_back(CreateBlizzard(map, p));
      }
    }
  }
  return blizzards;
}

[[nodiscard]] size_t Distance(Point a, Point b) noexcept {
  const auto [ax, ay] = a;
  const auto [bx, by] = b;
  return abs(ax - bx) + abs(ay - by);
}

[[nodiscard]] Point Position(const Blizzard &b, int t) noexcept {
  const auto tf = t % b.distance;
  const auto d = Distance(b.start, b.last) + 1;
  const bool is_restart = tf >= d;
  const auto start = is_restart ? b.first : b.start;
  const auto n = is_restart ? (tf - d) : tf;
  return Shift(start, b.shift, n);
}

[[nodiscard]] Points BlizzardsAt(const Blizzards &blizzards, int t) noexcept {
  Points current_positions;
  for (const auto &b : blizzards) {
    current_positions.insert(Position(b, t));
  }
  return current_positions;
}

// XXX only for debug purposes
[[nodiscard]] std::string Print(const Map &map,
                                const Points &blizzards) noexcept {
  std::string out;
  const int max_y = map.size();
  for (int y = 0; y < max_y; y++) {
    const auto &line = map.at(y);
    const int max_x = line.size();
    for (int x = 0; x < max_x; x++) {
      const Point p{x, y};
      char c = At(map, p);
      if (c != kWall) {
        c = blizzards.find(p) == blizzards.end() ? '.' : '*';
      }
      out += c;
    }
    out += '\n';
  }
  return out;
}

[[nodiscard]] int Answer1(const Map &map) noexcept {
  const auto blizzards = InitBlizzards(map);
  const auto index = [](std::string_view line) -> int {
    return line.find(kGround);
  };
  Point start{index(map[0]), 0};
  const int y_max = map.size() - 1;
  Point finish{index(map[y_max]), y_max};
  int t{0};
  Points frontline = {start};
  while (!frontline.empty()) {
    t++;
    const auto positions_blizzards = BlizzardsAt(blizzards, t);
    const auto out = Print(map, positions_blizzards);
    const auto is_free = [&map, &positions_blizzards](Point p) -> bool {
      return At(map, p) != kWall &&
             positions_blizzards.find(p) == positions_blizzards.end();
    };
    Points next;
    for (const Point &p0 : frontline) {
      // logic on current state the cell can be occupied by a blizzard
      // but it must be free on the next step
      // 5 options: up, down, right, left, stay
      for (const Point &s : kOptions) {
        const Point p1 = Shift(p0, s);
        if (p1 == finish)
          return t;
        if (is_free(p1)) {
          next.emplace(p1);
        }
      }
    }
    frontline = next;
  }
  return t;
}

[[nodiscard]] int Solution(std::string_view file) noexcept {
  return Answer1(ReadData(file));
}

} // namespace day24

TEST(AoC22, Day24) {
  EXPECT_EQ(day24::Solution("24-sample"), 18);
  // 147 min
  // 145 min, 152 too low, 160 too low (10%), 174 too low (20%)
  // 224 -- not right
  EXPECT_EQ(day24::Solution("24"), 0);
}
