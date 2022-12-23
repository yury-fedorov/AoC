#include "common.h"

namespace day22 {

using Point = std::pair<int, int>;
enum class Direction : int { kRight = 0, kDown, kLeft, kUp };

constexpr std::array kShifts = {Point{1, 0}, Point{0, 1}, Point{-1, 0},
                                Point{0, -1}};

[[nodiscard]] int64_t FinalPassword(int row, int column,
                                    Direction direction) noexcept {
  return (1000 * row) + (4 * column) + static_cast<int>(direction);
}

[[nodiscard]] std::pair<std::vector<std::string>, std::string>
Load(std::string_view file) noexcept {
  const auto data = ReadData(file);
  const auto map = data | rv::filter([](const std::string &s) {
                     return s.find('.') != std::string::npos;
                   }) |
                   r::to<std::vector>();
  const std::string path = *(data.rbegin() + 1); // very last line is empty??
  return {map, path};
}

} // namespace day22

TEST(AoC22, Day22) {
  const auto [map, path] = day22::Load("22");
  EXPECT_EQ(path, "");
  EXPECT_EQ(day22::FinalPassword(6, 8, day22::Direction::kRight), 6032);
}