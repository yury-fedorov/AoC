#ifndef AOC22_DAY22_H_
#define AOC22_DAY22_H_

#include <array>
#include <cstdint>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

namespace day22 {

using Point = std::pair<int, int>;
using Map = std::vector<std::string>;
enum class Direction : int { kRight = 0, kDown, kLeft, kUp };
using Position = std::pair<Point, Direction>;
using SegDir = std::pair<char, char>;

constexpr std::array kShifts = {Point{1, 0}, Point{0, 1}, Point{-1, 0},
                                Point{0, -1}};

constexpr std::array kDirectionChar = {'R', 'D', 'L', 'U'};

constexpr char EMPTY = ' ';
constexpr char WALL = '#';
constexpr char TILE = '.';
constexpr int kSegmentSize = 50;

[[nodiscard]] int64_t FinalPassword(int row, int column,
                                    Direction direction) noexcept;
[[nodiscard]] int64_t Answer1(std::string_view file) noexcept;
[[nodiscard]] int64_t Answer2(std::string_view file) noexcept;

} // namespace day22

#endif // AOC22_DAY22_H_