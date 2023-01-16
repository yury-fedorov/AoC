#include "common.h"

namespace day17 {

using Long = long long;
using Point = std::pair<int, Long>;
using Chamber = std::deque<std::string>;  // as we insert in the beginning
using Rock = std::vector<Point>;
constexpr int kChamberWidth = 7;

struct JetPattern {
  std::string_view pattern;
  size_t position;
  char next() noexcept {
    const char next = pattern[position];
    position = (position + 1) % pattern.length();
    return next;
  }
};

const std::vector kHLine = {Point{0, 0}, Point{1, 0}, Point{2, 0}, Point{3, 0}};
const std::vector kPlus = {Point{1, 0}, Point{0, 1}, Point{1, 1}, Point{2, 1},
                           Point{1, 2}};
const std::vector kLShape = {Point{2, 0}, Point{2, 1}, Point{2, 2}, Point{0, 2},
                             Point{1, 2}};
const std::vector kVLine = {Point{0, 0}, Point{0, 1}, Point{0, 2}, Point{0, 3}};
const std::vector kSquare = {Point{0, 0}, Point{0, 1}, Point{1, 0},
                             Point{1, 1}};

const std::vector<Rock> kRocks = {kHLine, kPlus, kLShape, kVLine, kSquare};

const std::array kRockHeightList = {1, 3, 3, 4, 2};

constexpr std::string_view kEmptyLine = ".......";
constexpr char kRockPiece = '#';

[[nodiscard]] bool IsEmpty(std::string_view line) noexcept {
  return !absl::StrContains(line, kRockPiece);
}

[[nodiscard]] Long CountLines(const Chamber &chamber) noexcept {
  return r::count_if(chamber,
                     [](std::string_view line) { return !IsEmpty(line); });
}

void SetRock(Chamber &chamber, Point point) noexcept {
  const auto [x, y] = point;
  chamber.at(y)[x] = kRockPiece;
}

[[nodiscard]] bool IsFeasible(const Chamber &chamber, Point point) noexcept {
  const auto [x, y] = point;
  if (chamber.size() >= y || y < 0 || x < 0) return false;
  const auto &line = chamber.at(y);
  if (line.size() >= x) return false;
  return line[x] == '.';
}

[[nodiscard]] bool IsFeasible(const Chamber &chamber, const Rock &rock,
                              Point shift) noexcept {
  const auto [dx, dy] = shift;
  for (const auto [x, y] : rock) {
    if (!IsFeasible(chamber, Point{x + dx, y + dy})) return false;
  }
  return true;
}

void FallOnePiece(Chamber &chamber, JetPattern &jet, int rock_id) noexcept {
  // Each rock appears so that its left edge is two units away from the left
  // wall and its bottom edge is three units above the highest rock in the room
  // (or the floor, if there isn't one).
  const Rock &rock = kRocks.at(rock_id);
  const int rock_height = kRockHeightList.at(rock_id);
  const int additional_height =
      3 + rock_height;  // we need to add to manipulate chamber
  const Long not_empty_lines = CountLines(chamber);
  const int to_add = additional_height - (chamber.size() - not_empty_lines);
  for (int i = 0; i < to_add; i++) {
    chamber.push_front(std::string(kEmptyLine));
  }
  int x = 2;   // left side
  Long y = 0;  // up side
  // now the chamber is fixed in size till the next piece
  while (true) {
    const int wind_direction = jet.next() == '<' ? -1 : 1;
    const auto x1 = x + wind_direction;
    if (IsFeasible(chamber, rock, {x1, y})) {
      x = x1;
    }
    const Long y1 = y + 1;
    if (!IsFeasible(chamber, rock, {x, y1})) {
      // It is time to materialize failing cube
      for (const auto &[px, py] : rock) {
        SetRock(chamber, {px + x, py + y});
      }
      break;
    }
    y = y1;
  }
}

[[nodiscard]] size_t Answer1(std::string_view jet_pattern_file) noexcept {
  const auto data = ReadData(jet_pattern_file).at(0);
  JetPattern jet{data, 0};
  Chamber chamber;
  const int rock_count = kRocks.size();
  for (int rock_id = 0; rock_id < 2022; rock_id++) {
    FallOnePiece(chamber, jet, rock_id % rock_count);
  }
  return CountLines(chamber);
}

}  // namespace day17

TEST(AoC22, Day17) {
  // XXX if (IsFastOnly()) return;  // TODO - no solution yet
  const auto answer1 = day17::Answer1("17-sample");
  EXPECT_EQ(answer1, 3068);
}
