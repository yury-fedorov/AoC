#include "common.h"
#include "re2/re2.h"

namespace day22 {

using Point = std::pair<int, int>;
using Map = std::vector<std::string>;
using Input = std::pair<Map, std::string>;
enum class Direction : int { kRight = 0, kDown, kLeft, kUp };
using Position = std::pair<Point, Direction>;

constexpr std::array kShifts = {Point{1, 0}, Point{0, 1}, Point{-1, 0},
                                Point{0, -1}};

constexpr char EMPTY = ' ';
constexpr char WALL = '#';
constexpr char TILE = '.';

[[nodiscard]] int64_t FinalPassword(int row, int column,
                                    Direction direction) noexcept {
  return (1000 * row) + (4 * column) + static_cast<int>(direction);
}

[[nodiscard]] Input Load(std::string_view file) noexcept {
  const auto data = ReadData(file);
  const auto map = data | rv::filter([](const std::string &s) {
                     return s.find(TILE) != std::string::npos;
                   }) |
                   r::to<std::vector>();
  const std::string path = *(data.rbegin());
  return {map, path};
}

class Navigator {
 protected:
  const Map &map_;

  Navigator(const Map &map) : map_(map) {}

  char At(int x, int y) const noexcept {
    if (y < 0 || x < 0 || y >= map_.size()) return EMPTY;
    const auto &line = map_[y];
    if (x >= line.size()) return EMPTY;
    // EXPECT_TRUE( line.size() == x_n ) << line.size() << " " << x_n << " " <<
    // line;
    return line[x];
  }

  virtual Position Overlap(Position from_direction) noexcept = 0;

 public:
  // basic navigation
  [[nodiscard]] Position To(Position from_direction) noexcept {
    auto [from, direction] = from_direction;
    const auto [dx, dy] = kShifts[static_cast<int>(direction)];
    const auto [x0, y0] = from;
    auto x1 = x0 + dx;
    auto y1 = y0 + dy;
    char c1 = At(x1, y1);
    if (c1 == ' ') {
      const auto [p, d] = Overlap(from_direction);
      x1 = p.first;
      y1 = p.second;
      direction = d;
      c1 = At(x1, y1);
    }
    if (c1 == WALL) return {from, direction};
    return {{x1, y1}, direction};
  }
};

class Navigator1 : public Navigator {
 protected:
  Position Overlap(Position from_direction) noexcept override {
    auto [from, direction] = from_direction;
    auto [x1, y1] = from;
    const auto [dx, dy] = kShifts[static_cast<int>(direction)];
    while (At(x1 - dx, y1 - dy) != EMPTY) {
      x1 -= dx;
      y1 -= dy;
    }
    return {{x1, y1}, direction};
  }

 public:
  Navigator1(const Map &map) : Navigator(map) {}
};

[[nodiscard]] int Answer1(std::string_view file) noexcept {
  const auto [map_binding, path] = day22::Load(file);
  const Map &map = map_binding;
  // You begin the path in the leftmost open tile of the top row of tiles.
  // Initially, you are facing to the right (from the perspective of how the map
  // is drawn).
  Direction facing = Direction::kRight;
  int y = 0;
  const std::string &line = map.front();
  int x = 0;
  const int y_n = map.size();
  const int x_n = line.size();
  for (; x < x_n; x++) {
    if (line[x] == TILE) break;
  }

  Navigator1 navigator(map);

  // found the start
  auto position = Point{x, y};
  static re2::RE2 re_digit("(\\d+)(.*)");
  static re2::RE2 re_turn("([RL])(.*)");
  std::string path_head = path;
  while (!path_head.empty()) {
    re2::StringPiece input(path_head);
    int moves{0};
    std::string turn, tail;
    if (re2::RE2::FullMatch(input, re_digit, &moves, &tail)) {
      // moves in direction
      for (int i = 0; i < moves; i++) {
        const auto [new_position, new_direction] =
            navigator.To({position, facing});
        position = new_position;
        facing = new_direction;
      }
    } else if (re2::RE2::FullMatch(input, re_turn, &turn, &tail)) {
      // turn
      const int turn_value = turn == "R" ? 1 : -1;
      int new_facing = (static_cast<int>(facing) + turn_value) % 4;
      new_facing += (new_facing < 0 ? 4 : 0);
      facing = static_cast<Direction>(new_facing);
    } else {
      EXPECT_TRUE(false) << path_head;
      break;
    }
    path_head = tail;
  }
  const auto [column, row] = position;
  return FinalPassword(row + 1, column + 1, facing);
}

// cube in personal map (it is not generic)
// 2R-7R 2D-4R 2U-9D
// 1U-9L 1L-6L
// 4R-2D 4L-6U
// 7R-2R 7D-9R
// 6U-4L 6L-1L

}  // namespace day22

TEST(AoC22, Day22) {
  EXPECT_EQ(day22::FinalPassword(6, 8, day22::Direction::kRight), 6032);
  EXPECT_EQ(day22::Answer1("22-sample"), 6032);
  EXPECT_EQ(day22::Answer1("22"), 27436);
  // TODO - part2
}
