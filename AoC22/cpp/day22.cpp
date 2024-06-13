#include "common.h"
#include "absl/algorithm/container.h"
#include "re2/re2.h"

namespace day22 {

using Point = std::pair<int, int>;
using Map = std::vector<std::string>;
enum class Direction : int { kRight = 0, kDown, kLeft, kUp };
using Position = std::pair<Point, Direction>;
using Answers = std::vector<int>;
using SegDir = std::pair<char, char>;

constexpr std::array kShifts = {Point{1, 0}, Point{0, 1}, Point{-1, 0},
                                Point{0, -1}};

constexpr std::array kDirectionChar = { 'R', 'D', 'L', 'U' };

constexpr char EMPTY = ' ';
constexpr char WALL = '#';
constexpr char TILE = '.';

[[nodiscard]] int64_t FinalPassword(int row, int column,
                                    Direction direction) noexcept {
  return (1000 * row) + (4 * column) + static_cast<int>(direction);
}

[[nodiscard]] std::pair<Map, std::string> Load(std::string_view file) noexcept {
  const auto data = ReadData(file);
  Map map;
  absl::c_copy_if(data, std::back_inserter(map), [](const std::string &s) {
    return s.find(TILE) != std::string::npos;
  });
  const std::string path = *(data.rbegin());
  return {map, path};
}

constexpr int kSegmentSize = 50;

constexpr Point PointToSegment(Point point,
                               int segment_size = kSegmentSize) noexcept {
  const auto [x, y] = point;
  return {x / segment_size, y / segment_size};
}

constexpr char ToDir(Direction d) noexcept {
  return kDirectionChar[static_cast<int>(d)];
}

constexpr Direction ToDirection(char d) noexcept {
  return static_cast<Direction>(d);
}

constexpr std::string_view kSegments = "123456789ABC";

constexpr char Tile(Point segment) noexcept {
  // 1 2 3
  // 4 5 6
  // 7 8 9
  // A B C
  const auto [x, y] = segment;
  const auto index = x + (y * 3);
  return kSegments[index];
}

constexpr SegDir ToSegDir(Point point, Direction d) noexcept {
  return {Tile(PointToSegment(point)), ToDir(d)};
}

constexpr std::optional<SegDir> NewSegDir(const std::string_view sdsd,
                                          SegDir sd) noexcept {
  if (SegDir{sdsd[0], sdsd[1]} == sd)
    return SegDir{sdsd[2], sdsd[3]};
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
  return {SegDir{from_to[2], Back(from_to[3])}, SegDir{from_to[0], from_to[1]}};
}

constexpr Point GlobalToSegment( Point global ) noexcept {
  const auto [x,y] = global;
  return { x % kSegmentSize, y % kSegmentSize };
}

constexpr Point SegmentToGlobal( Point local, char segment ) noexcept {
  const auto index = kSegments.find(segment);
  const auto di = div( index, 3 );
  const auto dy = di.quot, dx = di.rem; 
  const auto [x,y] = local;
  return { x + ( dx * kSegmentSize ), y + ( dy * kSegmentSize ) };
} 

constexpr Point Transform( Point point, char method ) noexcept {
  auto [x,y] = point;
  if ( method == 's' ) {
    std::swap(x,y);
  }
  return {x,y};
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
    if (c1 == ' ') {
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
    while (At(x1 - dx, y1 - dy) != EMPTY) {
      x1 -= dx;
      y1 -= dy;
    }
    return {{x1, y1}, direction};
  }

public:
  Navigator1(const Map &map) : Navigator(map) {}
};

class Navigator2 : public Navigator {
protected:
  // cube in personal map (it is not generic)
  // 2R-7R 2D-4R 2U-9D
  // 1U-9L 1L-6L
  // 4R-2D 4L-6U
  // 7R-2R 7D-9R
  // 6U-4L 6L-1L
  Position Overlap(Position from_direction) const noexcept override {
    const auto [from, direction] = from_direction;
    // TODO: implement part 2
    return {from, direction};
    // TODO - early draft
    // cube in personal map (it is not generic)
    constexpr std::array kMapping = {"2UARs", "AL2Ds"};

    // TODO - early draft
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

[[nodiscard]] Answers Solution(std::string_view file) noexcept {
  const auto [map_binding, path] = day22::Load(file);
  const Map &map = map_binding;
  // You begin the path in the leftmost open tile of the top row of tiles.
  // Initially, you are facing to the right (from the perspective of how the map
  // is drawn).
  int y = 0;
  const std::string &line = map.front();
  int x = 0;
  const int y_n = map.size();
  const int x_n = line.size();
  for (; x < x_n; x++) {
    if (line[x] == TILE)
      break;
  }
  // found the start
  const Point start{x, y};

  const Navigator1 navigator1(map);
  const Navigator2 navigator2(map);
  const std::vector<const Navigator *> kNovigators = {&navigator1, &navigator2};
  Answers answers;
  for (const Navigator *navigator : kNovigators) {
    Direction facing = Direction::kRight;
    auto position = start;
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
              navigator->To({position, facing});
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
    answers.push_back(FinalPassword(row + 1, column + 1, facing));
  }
  return answers;
}

} // namespace day22

TEST(AoC22, Day22) {
  EXPECT_EQ(day22::FinalPassword(6, 8, day22::Direction::kRight), 6032);
  const auto test = day22::Solution("22-sample");
  EXPECT_EQ(test[0], 6032);
  const auto answers = day22::Solution("22");
  EXPECT_EQ(answers[0], 27436);
  
  // TODO - Day22 Part2 to be solved
  if ( IsGreenOnly() ) return;
  
  EXPECT_EQ(test[1], 5031);
  EXPECT_EQ(answers[1], 0);
}
