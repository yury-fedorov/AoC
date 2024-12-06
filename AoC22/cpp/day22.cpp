#include "day22.h"
#include "common-std.h"
#include <cassert>
#include <format>
#include <functional>
#include <iostream>
#include <optional>
#include <regex>
#include <string>

namespace day22 {

[[nodiscard]] int64_t FinalPassword(int row, int column,
                                    Direction direction) noexcept {
  return (1000 * row) + (4 * column) + static_cast<int>(direction);
}

// Loading the map and the path
[[nodiscard]] std::pair<Map, std::string> Load(std::string_view file) noexcept {
  const auto data = ReadData(file);
  Map map;
  std::copy_if(
      data.begin(), data.end(), std::back_inserter(map),
      [](const std::string &s) { return s.find(TILE) != std::string::npos; });
  const std::string path = *(data.rbegin());
  return {map, path};
}

// Find the starting point.
[[nodiscard]] Point FindStart(const Map &map) noexcept {
  // You begin the path in the leftmost open tile of the top row of tiles.
  // Initially, you are facing to the right (from the perspective of how the map
  // is drawn).
  const std::string &line = map.front();
  const size_t x_n = line.size();
  int x = 0;
  for (; x < x_n; x++) {
    if (line[x] == TILE)
      break;
  }
  // found the start
  return Point{x, 0};
}

class Navigator {
protected:
  const Map &map_;

  Navigator(const Map &map) : map_(map) {}

  char At(int x, int y) const noexcept {
    if (y < 0 || x < 0 || y >= map_.size())
      return EMPTY;
    const auto &line = map_[y];
    if (x >= line.size())
      return EMPTY;
    // EXPECT_TRUE( line.size() == x_n ) << line.size() << " " << x_n << " " <<
    // line;
    return line[x];
  }

  virtual Position Overlap(Position from_direction) const noexcept = 0;

public:
  // basic navigation
  [[nodiscard]] Position To(Position from_direction) const noexcept {
    auto [from, direction] = from_direction;
    const auto [dx, dy] = kShifts[static_cast<int>(direction)];
    const auto [x0, y0] = from;
    auto x1 = x0 + dx;
    auto y1 = y0 + dy;
    char c1 = At(x1, y1);
    if (c1 == EMPTY) {
      // Special situation
      const auto [p, d] = Overlap(from_direction);
      x1 = p.first;
      y1 = p.second;
      direction = d;
      c1 = At(x1, y1);
    }
    if (c1 == WALL)
      return {from, direction};
    return {{x1, y1}, direction};
  }
};

class Navigator1 : public Navigator {
protected:
  Position Overlap(Position from_direction) const noexcept override {
    const auto [from, direction] = from_direction;
    auto [x1, y1] = from;
    const auto [dx, dy] = kShifts[static_cast<int>(direction)];
    // If a movement instruction would take you off of the map,
    // you wrap around to the other side of the board.
    while (At(x1 - dx, y1 - dy) != EMPTY) {
      x1 -= dx;
      y1 -= dy;
    }
    return {{x1, y1}, direction};
  }

public:
  Navigator1(const Map &map) : Navigator(map) {}
};

// Part 2 -------------------------------

// Converts global point to a segment.
constexpr Point PointToSegment(Point point,
                               int segment_size = kSegmentSize) noexcept {
  const auto [x, y] = point;
  return {x / segment_size, y / segment_size};
}

// Converts global point to local point within a segment.
Point PointToLocal(Point point, int segment_size = kSegmentSize) noexcept {
  const auto [x, y] = point;
  return Point{x % segment_size, y % segment_size};
}

constexpr int Tile(Point segment) noexcept {
  // 0 1 2
  // 3 4 5
  // 6 7 8
  // 9 A B
  const auto [x, y] = segment;
  return x + (y * 3);
}

constexpr int PointToTile(Point point,
                          int segment_size = kSegmentSize) noexcept {
  return Tile(PointToSegment(point, segment_size));
}

Point LocalToGlobal(Point local, int tile,
                    int segment_size = kSegmentSize) noexcept {
  const auto di = div(tile, 3);
  const int dy = (di.quot * segment_size), dx = (di.rem * segment_size);
  const auto [x, y] = local;
  return Point{x + dx, y + dy};
}

constexpr char ToDir(Direction d) noexcept {
  return kDirectionChar[static_cast<int>(d)];
}

constexpr Direction ToDirection(char d) noexcept {
  return static_cast<Direction>(
      std::find(kDirectionChar.begin(), kDirectionChar.end(), d) -
      kDirectionChar.begin());
}

constexpr Direction Back(Direction d) noexcept {
  return static_cast<Direction>((static_cast<int>(d) + 2) % 4);
}

class Navigator2 : public Navigator {
protected:
  // cube in personal map (it is not generic)
  //
  // . 1 2
  // . 4 .
  // 6 7 .
  // 9 . .
  //
  // Mappings which were not necessary due to the path:
  // 4R-2D 4L-6U 7R-2R 6L-1L
  Position Overlap(Position from_direction) const noexcept override {
    const auto [from, direction] = from_direction;
    auto [x0, y0] = from;
    const Point local0 = PointToLocal(from);
    // Tile and direction of the starting point
    const std::string td0 =
        std::format("{}{}", PointToTile(from), ToDir(direction));
    const int kMax = kSegmentSize - 1;

    // converters
    const auto U2L = [](const Point &local, int index) {
      const auto [x, _] = local;
      return LocalToGlobal(Point{0, x}, index);
    };
    const auto L2U = [](const Point &local, int index) {
      const auto [x, y] = local;
      return LocalToGlobal(Point{y, 0}, index);
    };
    const auto D2R = [](const Point &local, int index) {
      const auto [x, y] = local;
      return LocalToGlobal(Point{kMax, x}, index);
    };
    const auto R2D = [](const Point &local, int index) {
      const auto [_, y] = local;
      return LocalToGlobal(Point{y, kMax}, index);
    };
    const auto R2R = [](const Point &local, int index) {
      const auto [x, y] = local;
      return LocalToGlobal(Point{kMax, kMax - y}, index);
    };
    const auto U2D = [](const Point &local, int index) {
      const auto [x, y] = local; // ie. 2U-9D
      return LocalToGlobal(Point{x, kMax}, index);
    };
    const auto D2U = [](const Point &local, int index) {
      const auto [x, y] = local;
      return LocalToGlobal(Point{x, 0}, index);
    };
    const auto L2L = [](const Point &local, int index) {
      const auto [x, y] = local;
      return LocalToGlobal(Point{0, kMax - y}, index);
    };

    // Transform to the result (new global point and direction).
    const auto tx = [&local0](std::string_view to,
                              std::function<Point(Point, int)> tx) -> Position {
      const int index = stoi(std::string(to.substr(0, 1)));
      // the side where enters in the new tile
      const Direction direction = ToDirection(to[1]);
      // the direction is opposite of the side where enters
      const Direction opposite = Back(direction);
      return Position{tx(local0, index), opposite};
    };

    if (td0 == "1U") {
      return tx("9L", U2L); // 1U-9L
    } else if (td0 == "9L") {
      return tx("1U", L2U); // 9L-1U
    } else if (td0 == "2D") {
      return {D2R(local0, 4), Direction::kLeft}; // 2D-4R
    } else if (td0 == "4R") {
      return tx("2D", R2D); // 4R-2D
    } else if (td0 == "6U") {
      return {U2L(local0, 4), Direction::kRight}; // 6U-4L
    } else if (td0 == "4L") {
      return {L2U(local0, 6), Direction::kDown}; // 4L-6U
    } else if (td0 == "2R") {
      return {R2R(local0, 7), Direction::kLeft}; // 2R-7R
    } else if (td0 == "7R") {
      return {R2R(local0, 2), Direction::kLeft}; // 7R-2R
    } else if (td0 == "7D") {
      return {D2R(local0, 9), Direction::kLeft}; // 7D-9R
    } else if (td0 == "9R") {
      return tx("7D", R2D); // 9R-7D
    } else if (td0 == "2U") {
      return tx("9D", U2D); // 2U-9D
    } else if (td0 == "9D") {
      return tx("2U", D2U); // 9D-2U
    } else if (td0 == "1L") {
      return tx("6L", L2L); //  1L-6L
    } else if (td0 == "6L") {
      return tx("1L", L2L); //  6L-1L
    }

    // TODO: implement part 2
    std::cout << td0 << std::endl;
    assert(false);

    return {from, direction};
  }

public:
  Navigator2(const Map &map) : Navigator(map) {}
};

// Generic solution dependent from both navigator implementations
[[nodiscard]] int64_t Solution(std::string_view file, bool is_first) noexcept {
  auto [map, path] = Load(file);
  const Navigator1 navigator1(map);
  const Navigator2 navigator2(map);
  const Navigator *navigator =
      is_first ? (Navigator *)&navigator1 : &navigator2;
  Direction facing = Direction::kRight;
  auto position = FindStart(map);
  static std::regex re_digit("(\\d+)(.*)");
  static std::regex re_turn("([RL])(.*)");
  std::string path_head = path;
  while (!path_head.empty()) {
    int moves{0};
    std::string turn, tail;
    std::smatch what;
    // if (re2::RE2::FullMatch(input, re_digit, &moves, &tail)) {
    if (std::regex_match(path_head, what, re_digit)) {
      moves = stoi(what[1]);
      tail = what[2];
      // moves in direction
      for (int i = 0; i < moves; i++) {
        const auto [new_position, new_direction] =
            navigator->To({position, facing});
        position = new_position;
        facing = new_direction;
      }
      // } else if (re2::RE2::FullMatch(input, re_turn, &turn, &tail)) {
    } else if (std::regex_match(path_head, what, re_turn)) {
      turn = what[1];
      tail = what[2];
      // turn
      const int turn_value = turn == "R" ? 1 : -1;
      int new_facing = (static_cast<int>(facing) + turn_value) % 4;
      new_facing += (new_facing < 0 ? 4 : 0);
      facing = static_cast<Direction>(new_facing);
    } else {
      assert(false); // << path_head;
      break;
    }
    path_head = tail;
  }
  const auto [column, row] = position;
  return (FinalPassword(row + 1, column + 1, facing));
}

[[nodiscard]] int64_t Answer1(std::string_view file) noexcept {
  return Solution(file, true);
}

// too high: 158180
[[nodiscard]] int64_t Answer2(std::string_view file) noexcept {
  return Solution(file, false);
}

} // namespace day22
