#include "common.h"
#include <deque>
#include <map>
#include "absl/algorithm/container.h"

namespace day17 {

using Long = long long;
using Point = std::pair<int, Long>;
using Chamber = std::deque<std::string>;  // as we insert in the beginning
using Rock = std::vector<Point>;
constexpr int kChamberWidth = 7;
using Profile = uint64_t;  // byte for every cell

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
  return absl::c_count_if(chamber,
                     [](std::string_view line) { return !IsEmpty(line); });
}

void SetRock(Chamber &chamber, Point point) noexcept {
  const auto [x, y] = point;
  chamber.at(y)[x] = kRockPiece;
}

[[nodiscard]] bool IsFeasible(const Chamber &chamber, Point point) noexcept {
  const auto [x, y] = point;
  if (y >= chamber.size() || y < 0 || x < 0) return false;
  const auto &line = chamber.at(y);
  if (x >= line.size()) return false;
  return line[x] == '.';
}

[[nodiscard]] bool IsFeasible(const Chamber &chamber, const Rock &rock,
                              Point shift) noexcept {
  const auto [dx, dy] = shift;
  for (const auto [x, y] : rock) {
    const bool is_feasible = IsFeasible(chamber, Point{x + dx, y + dy});
    if (!is_feasible) return false;
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
  int x = 2;   // left side
  Long y = 0;  // up side
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
  const auto chamber_depth{chamber.size()};
  constexpr uint8_t kMaxDepth{30};
  const uint8_t max_depth = chamber_depth > kMaxDepth
                                ? kMaxDepth
                                : static_cast<uint8_t>(chamber_depth);
  for (int x = 0; x < kChamberWidth; x++) {
    uint8_t depth{0};
    for (; depth < max_depth && chamber[depth][x] != kRockPiece; depth++) {
    }
    profile = (profile << 8) | depth;
  }
  return profile;
}

using State =
    std::tuple<uint16_t, uint8_t, Profile>;  // position - 10092 long line,
                                             // rock_id 5 values, profile
using StateMark = std::tuple<Long, Long>;    // rock_id, lines
using Cache = std::map<State, StateMark>;

[[nodiscard]] std::string FormatProfile(Profile profile) noexcept {
  return absl::StrCat(absl::Hex(profile, absl::kZeroPad4));
}

[[nodiscard]] std::string FormatState(State state) noexcept {
  const auto [gas_pos, rid, prof] = state;
  return absl::StrCat("gas: ", gas_pos, " rid: ", rid,
                      " prof: ", FormatProfile(prof));
}

[[nodiscard]] std::string FormatMark(StateMark mark) noexcept {
  const auto [rock_id, chamber_height] = mark;
  return absl::StrCat("rock id: ", rock_id,
                      " chamber height: ", chamber_height);
}

[[nodiscard]] size_t Answer2(std::string_view jet_pattern_file) noexcept {
  const auto data = ReadData(jet_pattern_file).at(0);
  constexpr Long n{1'000'000'000'000LL};
  JetPattern jet{data, 0};
  Chamber chamber;
  Cache cache;
  const uint8_t rock_count = kRocks.size();
  Long loop_size{0};
  Long loop_count{0};
  Long dh{0};
  bool is_loop_found = false;
  for (Long rock_id = 0; rock_id < n; rock_id++) {
    const uint8_t small_rock_id = rock_id % rock_count;
    FallOnePiece(chamber, jet, small_rock_id);
    if (!is_loop_found) {
      const Profile profile{GetProfile(chamber)};
      const State state{jet.position, small_rock_id, profile};
      const Long cur_height{CountLines(chamber)};
      // StateMark mark{rock_id, cur_height};
      const auto [iterator, is_inserted] = cache.try_emplace(state, rock_id, cur_height);
      if (!is_inserted) {
        // now we may skip some
        const auto m0 = iterator->second;  // what position was hitted
        EXPECT_TRUE(false) << " profile: " << FormatProfile(profile)
                           << " hitted key: " << FormatState(iterator->first)
                           << " current key: " << FormatState(state) << " MARK "
                           << FormatMark(iterator->second);
        const auto [rid, lines] = m0;
        const StateMark *p_mark = nullptr;
        for (const auto &[_, v] : cache) {
          if (std::get<0>(v) == (rid + 1)) {
            p_mark = &v;
            break;
          }
        }
        EXPECT_TRUE(std::get<1>(*p_mark) >= lines) << "Height is expected higher";
        // loop_size = (rock_id - rid);
        loop_size = (rock_id - rid - 1);
        loop_count = n / loop_size;
        dh = cur_height - std::get<1>(*p_mark) + 1;
        const auto head = cur_height - dh;
        EXPECT_TRUE(false) << "loop size: " << loop_size
                           << " loop_count: " << loop_count << " head: " << head
                           << " dh: " << dh
                           << " profile: " << FormatProfile(profile)
                           << "cur height: " << cur_height
                           << " cur_rock_id: " << rock_id;
        rock_id = head + (loop_count * loop_size);
        is_loop_found = true;
      }
    }
  }
  return CountLines(chamber) + ((loop_count - 1LL) * dh);
}

}  // namespace day17

TEST(AoC22, Day17) {
  // XXX if (IsFastOnly()) return;  // TODO - no solution yet
  EXPECT_EQ(day17::Answer1("17-sample"), 3068);
  EXPECT_EQ(day17::Answer1("17"), 3217);
  if ( IsGreenOnly() ) return; // TODO - Day17 Part2 to be solved.
  EXPECT_EQ(day17::Answer2("17-sample"), 1514285714288LL);
}
