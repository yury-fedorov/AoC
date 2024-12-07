#ifndef AOC22_DAY16_H_
#define AOC22_DAY16_H_

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

// using Door = std::string_view;
using Door = std::string;
using Doors = std::vector<Door>;
using DoorSet = std::set<Door>;

// valve - configuration(rate, next doors)
struct Map : public std::map<std::string, Conf> {
  Doors all_doors;
  Doors openable;
  void Init();
  bool IsOpenable(const Door &door) const {
    return std::find(openable.begin(), openable.end(), door) != openable.end();
  }
};

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

#endif // AOC22_DAY16_H_