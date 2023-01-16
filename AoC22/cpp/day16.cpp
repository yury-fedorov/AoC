#include <queue>

#include "absl/container/flat_hash_map.h"
#include "absl/container/flat_hash_set.h"
#include "absl/strings/str_join.h"
#include "common.h"
#include "re2/re2.h"

namespace day16 {

struct Conf {
  int rate;
  std::vector<std::string> next;
};

using Map = absl::flat_hash_map<std::string, Conf>;
using Doors = std::vector<std::string_view>;  // open doors
using DoorSet = absl::flat_hash_set<std::string_view>;

constexpr int kT = 30;

[[nodiscard]] long PressureInMinute(const Map &map,
                                    const Doors &open) noexcept {
  long out = 0;
  for (const auto &ov : open) {
    out += map.at(ov).rate;
  }
  return out;
}

[[nodiscard]] Doors AllDoors(const Map &map) noexcept {
  Doors doors;
  r::for_each(map, [&doors](const auto &pair) { doors.push_back(pair.first); });
  return doors;
}

[[nodiscard]] int Distance(const Map &map, std::string_view from,
                           std::string_view to) noexcept {
  int distance{0};
  DoorSet frontline{from};
  while (frontline.find(to) == frontline.end()) {
    DoorSet next;
    for (const auto &door : frontline) {
      const auto &next_doors = map.at(door).next;
      next.insert(next_doors.begin(), next_doors.end());
    }
    distance++;
    frontline = std::move(next);
  }
  return distance;
}

// to avoid costly path searches over and over again
[[nodiscard]] int DistanceFast(const Map &map, std::string_view from,
                               std::string_view to) noexcept {
  using Input = std::pair<std::string_view, std::string_view>;
  using Cache = absl::flat_hash_map<Input, int>;
  static Cache cache;
  const Input input{from, to};
  const auto i = cache.find(input);
  if (i != cache.end()) return i->second;  // cache hitted
  const int distance = Distance(map, from, to);
  cache[input] = distance;
  return distance;
}

[[nodiscard]] Doors Sequence(const Map &map, std::string_view from,
                             std::string_view to, int depth) noexcept {
  if (from != to && depth > 0) {
    const auto &next_doors = map.at(from).next;
    if (absl::c_find(next_doors, to) != next_doors.end()) return Doors{to};
    // we are here because one level depth was not enough
    for (const auto &door : next_doors) {
      const auto d1 = depth - 1;
      if (d1 > 0) {
        auto s = Sequence(map, door, to, d1);
        if (!s.empty() && s.back() == to) {
          s.insert(s.begin(), door);
          return s;
        }
      }
    }
  }
  return Doors{};
}

[[nodiscard]] Doors SequenceFast(const Map &map, std::string_view from,
                                 std::string_view to) noexcept {
  using Input = std::pair<std::string_view, std::string_view>;
  using Cache = absl::flat_hash_map<Input, Doors>;
  static Cache cache;
  const Input input{from, to};
  const auto i = cache.find(input);
  if (i != cache.end()) return i->second;  // cache hitted
  const int distance = DistanceFast(map, from, to);
  const auto s = Sequence(map, from, to, distance);
  cache[input] = s;
  return s;
}

void RemoveOpened(Doors &doors, const Doors &open) noexcept {
  // erase-remove idiom
  absl::c_for_each(open, [&doors](const auto &door) {
    doors.erase(std::remove(doors.begin(), doors.end(), door), doors.end());
  });
}

void RemoveNoPressure(Doors &doors, const Map &map) noexcept {
  absl::c_for_each(map, [&doors](const auto &pair) {
    if (pair.second.rate == 0) {
      doors.erase(absl::c_find(doors, pair.first));
    }
  });
}

/*
// highest rates at the beginning with shortest path to it
void Order(const Map &map, Doors &doors, std::string_view from) noexcept {
  absl::c_sort(doors, [&map, from](const auto &a, const auto &b) {
    const int ad = DistanceFast(map, from, a);
    const int bd = DistanceFast(map, from, b);
    return (map.at(a).rate - ad) > (map.at(b).rate - bd);
  });
}
*/

[[nodiscard]] long Pressure(const Map &map, std::string_view start,
                            const Doors &open, int t_left,
                            std::optional<std::string_view> target) noexcept {
  if (t_left <= 0) return 0;
  const auto last_minute = PressureInMinute(map, open);
  if (t_left == 1) {
    // no sense to open anything, just calculate what is opened
    return last_minute;
  }
  // we've more then 1 step to go, opening a door could make sense
  if (target.has_value()) {
    // we go to the selected target
    const auto path = SequenceFast(map, start, target.value());
    const auto dt = path.size() + 1;  // 1 - to open the door
    const auto t1 = t_left - dt;
    Doors open1{open};
    open1.push_back(target.value());
    const auto pressure =
        Pressure(map, target.value(), open1, t1, std::nullopt);
    return pressure + (dt * last_minute);
  }

  // we do not have a selected target, we choose among the doors to open
  // 1. get all doors
  Doors doors = AllDoors(map);
  // 2. remove opened doors
  RemoveOpened(doors, open);
  // 3. remove no pressure doors
  RemoveNoPressure(doors, map);
  // 4. order first high rated doors (could try to optimize: top 50%)
  // Order(map, doors, start);
  long pressure{0};
  for (const auto &target : doors) {
    const long cur_pressure = Pressure(map, start, open, t_left, target);
    pressure = std::max(pressure, cur_pressure);
  }
  return pressure;
}

// open doors in format ,AA, ... ,ZZ,...
[[nodiscard]] long Answer1(std::string_view file) noexcept {
  const auto data = ReadData(file);
  // Valve HH has flow rate=22; tunnel leads to valve GG
  // Valve AA has flow rate=0; tunnels lead to valves DD, II, BB
  re2::RE2 re(
      "Valve (.+) has flow rate=(\\d+); tunnel.? lead.? to valve.? (.+)");
  Map map;
  for (const auto &line : data) {
    re2::StringPiece input(line);
    std::string valve, list;
    int rate{0};
    if (re2::RE2::FullMatch(input, re, &valve, &rate, &list)) {
      const std::vector<std::string> n = absl::StrSplit(list, ", ");
      map.insert({valve, Conf{rate, n}});
    } else
      EXPECT_TRUE(false) << line;
  }
  return Pressure(map, "AA", Doors{}, kT, std::nullopt);
}

}  // namespace day16

TEST(AoC22, Day16) {
  // return;  // TODO - wrong answer
  const auto a1 = day16::Answer1("16-sample");
  EXPECT_EQ(a1, 1651);
}
