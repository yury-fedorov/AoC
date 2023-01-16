#include "absl/container/flat_hash_map.h"
#include "common.h"

namespace day17 {

using Long = long long;
using Point = std::pair<int, Long>;
using Chamber = std::deque<std::string>; // as we insert in the beginning
using Rock = std::vector<Point>;
constexpr int kChamberWidth = 7;
using Profile = std::array<uint8_t, kChamberWidth>;

struct JetPattern {
  std::string_view pattern;
  uint16_t position;
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
  if (y >= chamber.size() || y < 0 || x < 0)
    return false;
  const auto &line = chamber.at(y);
  if (x >= line.size())
    return false;
  return line[x] == '.';
}

[[nodiscard]] bool IsFeasible(const Chamber &chamber, const Rock &rock,
                              Point shift) noexcept {
  const auto [dx, dy] = shift;
  for (const auto [x, y] : rock) {
    const bool is_feasible = IsFeasible(chamber, Point{x + dx, y + dy});
    if (!is_feasible)
      return false;
  }
  return true;
}

void FallOnePiece(Chamber &chamber, JetPattern &jet, int rock_id) noexcept {
  // Each rock appears so that its left edge is two units away from the left
  // wall and its bottom edge is three units above the highest rock in the room
  // (or the floor, if there isn't one).
  const Rock &rock = kRocks.at(rock_id);
  const int rock_height = kRockHeightList.at(rock_id);
  const int additional_height = 3 + rock_height;
  for (int i = 0; i < additional_height; i++) {
    chamber.push_front(std::string(kEmptyLine));
  }
  int x = 2;  // left side
  Long y = 0; // up side
  // now the chamber is fixed in size till the next piece
  while (true) {
    const int gas_direction = jet.next() == '<' ? -1 : 1;
    // gas pushes rock (left or right)
    const auto x1 = x + gas_direction;
    if (IsFeasible(chamber, rock, {x1, y})) {
      x = x1;
    }
    const Long y1 = y + 1;
    if (!IsFeasible(chamber, rock, {x, y1})) {
      // It is time to materialize failing cube
      for (const auto &[px, py] : rock) {
        SetRock(chamber, {px + x, py + y});
      }
      // remove all empty lines
      while (IsEmpty(chamber.front())) {
        chamber.pop_front();
      }
      break;
    }
    // rock falls one unit
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

[[nodiscard]] Profile GetProfile(const Chamber &chamber) noexcept {
  Profile profile{0};
  const uint8_t max_depth =
      static_cast<uint8_t>(std::min(255LL, static_cast<Long>(chamber.size())));
  for (int x = 0; x < kChamberWidth; x++) {
    uint8_t depth{0};
    for (; depth < max_depth && chamber[depth][x] != kRockPiece; depth++) {
    }
    profile[x] = depth;
  }
  return profile;
}

using State =
    std::tuple<uint16_t, uint8_t, Profile>; // position - 10092 long line,
                                            // rock_id 5 values, profile
using StateMark = std::pair<Long, Long>;    // rock_id, lines
using Cache = absl::flat_hash_map<State, StateMark>;

[[nodiscard]] size_t Answer2(std::string_view jet_pattern_file) noexcept {
  const auto data = ReadData(jet_pattern_file).at(0);
  constexpr Long n{1'000'000'000'000LL};
  JetPattern jet{data, 0};
  Chamber chamber;
  Cache cache;
  const uint8_t rock_count = kRocks.size();
  Long loop_size{0};
  Long loop_count{0};
  for (Long rock_id = 0; rock_id < n; rock_id++) {
    const uint8_t small_rock_id = rock_id % rock_count;
    FallOnePiece(chamber, jet, small_rock_id);
    const State state{jet.position, small_rock_id, GetProfile(chamber)};
    const StateMark mark{rock_id, CountLines(chamber)};
    const auto [iterator, is_inserted] = cache.try_emplace(state, mark);
    if (!is_inserted) {
      // now we may skip some
      const auto m0 = iterator->second; // what position was hitted
      loop_size = cache.size();
      loop_count = n / loop_size;
      rock_id = loop_count * loop_size;
    }
  }
  return CountLines(chamber) + ((loop_count - 1) * loop_size);
}

} // namespace day17

TEST(AoC22, Day17) {
  // XXX if (IsFastOnly()) return;  // TODO - no solution yet
  EXPECT_EQ(day17::Answer1("17-sample"), 3068);
  EXPECT_EQ(day17::Answer1("17"), 3217);
  EXPECT_EQ(day17::Answer2("17-sample"), 1514285714288LL);
}
