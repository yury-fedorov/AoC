#ifndef AOC24_DAY16_H_
#define AOC24_DAY16_H_

#include <map>
#include <set>
#include <string>
#include <string_view>
#include <vector>

namespace day16 {

struct Conf {
  int rate;
  std::vector<std::string> next;
};

using Map = std::map<std::string, Conf>;
// using Door = std::string_view;
using Door = std::string;
using Doors = std::vector<Door>;
using DoorSet = std::set<Door>;

// input
[[nodiscard]] Map ReadMap(std::string_view file) noexcept;
// solution
[[nodiscard]] long Answer1(const Map &map) noexcept;
[[nodiscard]] long Answer2(const Map &map) noexcept;
// internal details
[[nodiscard]] long Pressure(const Map &map, Door cur_door, Doors open,
                            int t_max) noexcept;

const day16::Door kStart = "AA";
} // namespace day16

#endif // AOC24_DAY16_H_