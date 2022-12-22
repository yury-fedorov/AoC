#include "common.h"
namespace day17 {
using Long = long long;
using Point = std::pair<int, int>;
using Chamber = std::vector<std::string>;
constexpr int k_chamber_width = 7;

struct JetPattern {
  std::string_view pattern;
  size_t position;
  char next() noexcept {
    const char next = pattern[position];
    position = (position + 1) % pattern.length();
    return next;
  }
};

const std::vector k_h_line = {Point{0, 0}, Point{1, 0}, Point{2, 0},
                              Point{3, 0}};
const std::vector k_plus = {Point{1, 0}, Point{0, 1}, Point{1, 1}, Point{2, 1},
                            Point{1, 2}};
const std::vector k_l_shape = {Point{2, 0}, Point{2, 1}, Point{2, 2},
                               Point{0, 2}, Point{1, 2}};
const std::vector k_v_line = {Point{0, 0}, Point{0, 1}, Point{0, 2},
                              Point{0, 3}};
const std::vector k_square = {Point{0, 0}, Point{0, 1}, Point{1, 0},
                              Point{1, 1}};
const std::vector<std::vector<Point>> k_rocks = {k_h_line, k_plus, k_l_shape,
                                                 k_v_line, k_square};

void FallOnePiece(Chamber &chamber, JetPattern &jet) noexcept {}

size_t Answer1(std::string_view jet_pattern_file) noexcept {
  const auto data = ReadData(jet_pattern_file).at(0);
  JetPattern jet{data, 0};
  Chamber chamber;
  for (int rock_id = 0; rock_id < 2022; rock_id++) {
    FallOnePiece(chamber, jet);
  }
  return chamber
      .size(); // assumption that chamber lines are only filled by figures
}
} // namespace day17

TEST(AoC22, Day17) {
  if (IsFastOnly())
    return; // TODO - no solution yet
  const auto answer1 = day17::Answer1("17-sample");
  EXPECT_EQ(answer1, 3068);
}
