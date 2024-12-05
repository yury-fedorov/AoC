#include "day22.h"
#include "common-std.h"
#include <cassert>
#include <iostream>
#include <optional>
#include <regex>

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

// constexpr std::string_view kSegments = "0123456789AB";

constexpr int Tile(Point segment) noexcept {
    // 0 1 2
    // 3 4 5
    // 6 7 8
    // 9 A B
    const auto [x, y] = segment;
    return x + (y * 3);
}

/*
Point LocalToGlobal( localpoint, segment) noexcept {
    // TODO
}

constexpr char ToDir(Direction d) noexcept {
    return kDirectionChar[static_cast<int>(d)];
}

constexpr Direction ToDirection(char d) noexcept {
    return static_cast<Direction>(d);
}

constexpr SegDir ToSegDir(Point point, Direction d) noexcept {
    return { Tile(PointToSegment(point)), ToDir(d) };
}

constexpr std::optional<SegDir> NewSegDir(const std::string_view sdsd,
    SegDir sd) noexcept {
    if (SegDir{ sdsd[0], sdsd[1] } == sd)
        return SegDir{ sdsd[2], sdsd[3] };
    return std::nullopt;
}

constexpr Direction Back(Direction d) noexcept {
    return static_cast<Direction>((static_cast<int>(d) + 2) % 4);
}

constexpr char Back(char direction) noexcept {
    return ToDir(Back(ToDirection(direction)));
}

constexpr std::pair<SegDir, SegDir>
Back(const std::string_view from_to) noexcept {
    return { SegDir{from_to[2], Back(from_to[3])}, SegDir{from_to[0],
from_to[1]} };
}

Point GlobalToSegment(Point global) noexcept {
    const auto [x, y] = global;
    return { x % kSegmentSize, y % kSegmentSize };
}

Point SegmentToGlobal(Point local, char segment) noexcept {
    const auto index = kSegments.find(segment);
    const auto di = div(index, 3);
    const auto dy = di.quot, dx = di.rem;
    const auto [x, y] = local;
    return { x + (dx * kSegmentSize), y + (dy * kSegmentSize) };
}

constexpr Point Transform(Point point, char method) noexcept {
    auto [x, y] = point;
    if (method == 's') {
        std::swap(x, y);
    }
    return { x, y };
}
*/

class Navigator2 : public Navigator {
protected:
  // cube in personal map (it is not generic)
  // 
  // . 1 2
  // . 4 .
  // 6 7 .
  // 9 . .
  // 
  // 2R-7R 2D-4R 2U-9D
  // 1U-9L 1L-6L
  // 4R-2D 4L -6U
  // 7R-2R 7D-9R
  // 6U-4L 6L-1L
  Position Overlap(Position from_direction) const noexcept override {
    if (true) {
      // XXX: quick stub to make it compile
      const auto [from, direction] = from_direction;
      auto [x0, y0] = from;
      const auto [dx, dy] = kShifts[static_cast<int>(direction)];
      const auto x1 = x0 + dx;
      const auto y1 = y0 + dy;
      auto segment0 = PointToSegment({x0, y0});
      auto segment1 = PointToSegment({x1, y1});
      const auto [lx0, ly0] = PointToLocal(from);
      const int tile0 = Tile(segment0);
      if ( tile0 == 1 && direction == Direction::kUp) {
        // TODO: to 6 (L)
        return {Point{0, lx0}, Direction::kRight};
      }
      // TODO: implement part 2
      return {from, direction};
    }
    /* TODO - early draft
        constexpr std::array kMapping = { "2UAR", "AL2D" };
        // only for the real case
        constexpr int kss = 50;

        auto [from, direction] = from_direction;
        // global
        auto [xg, yg] = from;
        // local
        int xl = xg / kss;
        int yl = yg / kss;
        if ( yl == 0 && xl == 1 && direction == Direction::kUp ) {
          // 2U -> AL swap
          std::swap(xl, yl);
          xg = xl;
          yg = yl + (3 * kss);
          direction = Direction::kRight;
        } else if ( xl == 0 && yl == 3 && direction == Direction::kLeft ) {
          // AL -> 2U swap
          std::swap(xl, yl);
          xg = xl + (kss);
          yg = yl;
          direction = Direction::kDown;
        } else {
          std::cerr << xg << " " << yg << " " << (int)direction << " ";
          throw;
        }
        // return {{-1, -1}, direction};
        return {{xg, yg}, direction};
      */

    /*
    // TODO - early draft - https://github.com/yury-fedorov/AoC/pull/123 -  Jul
    9, 2023
    // cube in personal map (it is not generic)
    constexpr std::array kMapping = {"2UARs", "AL2Ds"};

    auto [from, direction] = from_direction;
    // global
    auto [xg, yg] = from;
    // segment
    auto segdir = ToSegDir(from, direction);
    for (auto &sdsdt : kMapping) {
      const auto nsd = NewSegDir(sdsdt, segdir);
      if (!nsd.has_value())
        continue;
      const auto local_point = GlobalToSegment(from);
      // transformation method ie swap
      // [lx1,ly1] Transform( local, method )
      // [gx1,gy1] SegmentToGlobal(local, segment)
      // return { {gx1, gy1}, nsd.direction }
    }
    // missing mapping
    std::cerr << xg << " " << yg << " " << (int)direction << " ";
    */
    /*
    int xl = xg / kss;
    int yl = yg / kss;
    if ( yl == 0 && xl == 1 && direction == Direction::kUp ) {
      // 2U -> AL swap
      std::swap(xl, yl);
      xg = xl;
      yg = yl + (3 * kss);
      direction = Direction::kRight;
    } else if ( xl == 0 && yl == 3 && direction == Direction::kLeft ) {
      // AL -> 2U swap
      std::swap(xl, yl);
      xg = xl + (kss);
      yg = yl;
      direction = Direction::kDown;
    } else {
      std::cerr << xg << " " << yg << " " << (int)direction << " ";
      throw;
    }
    // return {{-1, -1}, direction};
    return {{xg, yg}, direction};
  */
    /*
    auto [from, direction] = from_direction;
    // global
    auto [xg, yg] = from;
    // segment
    auto segdir = ToSegDir(from, direction);
    for (auto &sdsdt : kMapping) {
      const auto nsd = NewSegDir(sdsdt, segdir);
      if (!nsd.has_value())
        continue;
      const auto local_point = GlobalToSegment(from);
      // transformation method ie swap
      // [lx1,ly1] Transform( local, method )
      // [gx1,gy1] SegmentToGlobal(local, segment)
      // return { {gx1, gy1}, nsd.direction }
    }
    // missing mapping
    std::cerr << xg << " " << yg << " " << (int)direction << " ";

    /*
    int xl = xg / kss;
    int yl = yg / kss;
    if ( yl == 0 && xl == 1 && direction == Direction::kUp ) {
      // 2U -> AL swap
      std::swap(xl, yl);
      xg = xl;
      yg = yl + (3 * kss);
      direction = Direction::kRight;
    } else if ( xl == 0 && yl == 3 && direction == Direction::kLeft ) {
      // AL -> 2U swap
      std::swap(xl, yl);
      xg = xl + (kss);
      yg = yl;
      direction = Direction::kDown;
    } else {
      std::cerr << xg << " " << yg << " " << (int)direction << " ";
      throw;
    }
    // return {{-1, -1}, direction};
    return {{xg, yg}, direction};
  */
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

[[nodiscard]] int64_t Answer2(std::string_view file) noexcept {
  return Solution(file, false);
}

} // namespace day22
