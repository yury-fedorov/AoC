#include "common.h"
#include "re2/re2.h"

namespace day22 {

using Point = std::pair<int, int>;
using Map = std::vector<std::string>;
enum class Direction : int { kRight = 0, kDown, kLeft, kUp };

constexpr std::array kShifts = {Point{1, 0}, Point{0, 1}, Point{-1, 0},
                                Point{0, -1}};

constexpr char EMPTY = ' ';
constexpr char WALL = '#';
constexpr char TILE = '.';

[[nodiscard]] int64_t FinalPassword(int row, int column,
                                    Direction direction) noexcept {
  return (1000 * row) + (4 * column) + static_cast<int>(direction);
}

[[nodiscard]] std::pair<Map, std::string> Load(std::string_view file) noexcept {
  const auto data = ReadData(file);
  const auto map = data | rv::filter([](const std::string &s) {
                     return s.find( TILE ) != std::string::npos;
                   }) |
                   r::to<std::vector>();
  const std::string path = *(data.rbegin());
  return {map, path};
}

[[nodiscard]] int Answer1(std::string_view file) noexcept {
  const auto [map_binding, path] = day22::Load(file);
  const day22::Map &map = map_binding;
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
    if (line[x] == TILE)
      break;
  }
  const auto at = [&map, y_n](int x, int y) -> char {
    if (y < 0 || x < 0 || y >= y_n) return EMPTY;
    const auto & line = map[y];
    if ( x >= line.size() ) return EMPTY;
    // EXPECT_TRUE( line.size() == x_n ) << line.size() << " " << x_n << " " << line; 
    return line[x];
  };
  const auto to = [&map, &at](Point from, Direction direction) -> Point {
    const auto [dx, dy] = kShifts[static_cast<int>(direction)];
    const auto [x0, y0] = from;
    auto x1 = x0 + dx;
    auto y1 = y0 + dy;
    char c1 = at(x1, y1);
    if (c1 == ' ') {
      // opposite side
      x1 = x0;
      y1 = y0;
      while (at(x1- dx, y1 - dy) != EMPTY ) {
        x1 -= dx;
        y1 -= dy;
      }
      c1 = at(x1, y1);
    }
    if (c1 == WALL )
      return from;
    return {x1, y1};
  };

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
        position = to(position, facing);
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

} // namespace day22

TEST(AoC22, Day22) {
  EXPECT_EQ(day22::FinalPassword(6, 8, day22::Direction::kRight), 6032);
  EXPECT_EQ(day22::Answer1("22-sample"), 6032);
  // TODO - 
  EXPECT_EQ(day22::Answer1("22"), 27436);
}