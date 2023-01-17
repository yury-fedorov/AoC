#include <deque>
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
using Doors = std::vector<std::string_view>; // open doors
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
  if (i != cache.end())
    return i->second; // cache hitted
  const int distance = Distance(map, from, to);
  cache[input] = distance;
  return distance;
}

[[nodiscard]] Doors Sequence(const Map &map, std::string_view from,
                             std::string_view to, int depth) noexcept {
  if (from != to && depth > 0) {
    const auto &next_doors = map.at(from).next;
    if (absl::c_find(next_doors, to) != next_doors.end())
      return Doors{to};
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
  if (i != cache.end())
    return i->second; // cache hitted
  const int distance = DistanceFast(map, from, to);
  const auto s = Sequence(map, from, to, distance);
  cache[input] = s;
  return s;
}

void RemoveOpened(Doors &doors, const Doors &open) noexcept {
  // erase-remove idiom
  absl::c_for_each(open, [&doors](const auto &door) {
    //    doors.erase(std::remove(doors.begin(), doors.end(), door),
    //    doors.end());
    doors.erase(absl::c_find(doors, door));
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

/*
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
    if (t1 <= 0) return t_left * last_minute;
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
  Order(map, doors, start);
  long pressure{t_left * last_minute};
  for (const auto &target : doors) {
    const long cur_pressure = Pressure(map, start, open, t_left, target);
    pressure = std::max(pressure, cur_pressure);
  }
  return pressure;
}
*/

/*
[[nodiscard]] long Pressure(const Map &map, std::string_view cur_door,
                            std::deque<std::string_view> sequence,
                            int t_max) noexcept {
  long pressure{0};
  Doors open;
  for (int t{0}; t < t_max;) {
    const auto last_minute = PressureInMinute(map, open);
    int dt;
    if (sequence.empty()) {
      dt = t_max;
    } else {
      const auto target = sequence.front();
      sequence.pop_front();
      const auto path = SequenceFast(map, cur_door, target);
      dt = path.size() + 1;  // 1 - to open the door
      open.push_back(target);
      cur_door = target;
    }
    const auto t1{t + dt};
    const auto edt = t1 < t_max ? dt : (t_max - t);
    pressure += edt * last_minute;
    t += edt;
    // EXPECT_TRUE(false) << "t=" << t << " pressure= " << pressure << " cur
    // door = " << target;
  }
  return pressure;
}
*/

[[nodiscard]] long Pressure(const Map &map, std::string_view cur_door,
                            Doors open, int t_max) noexcept {
  if (t_max < 0)
    return 0;
  const auto last_minute = PressureInMinute(map, open);
  // 1. get all doors
  Doors doors = AllDoors(map);
  // 2. remove opened doors
  RemoveOpened(doors, open);
  // 3. remove no pressure doors
  RemoveNoPressure(doors, map);
  /*
  constexpr bool kIsOptimized = false;
  if (kIsOptimized) {
    // 4. order first high rated doors (could try to optimize: top 50%)
    Order(map, doors, cur_door);
    const auto n{doors.size()};
    // 4379 - not right answer -- 639s
    constexpr size_t kMaxHead =
        5;  // with 3 - 3735 (too high) 4 - 4149 (too high), 5 - 4149, no
            // restrict - 4442 -- too high? - 635s
    if (n > kMaxHead) {
      doors.resize(kMaxHead);
    }
  }
  */
  long pressure{t_max * last_minute};
  for (const auto &target : doors) {
    // const auto path = SequenceFast(map, cur_door, target);
    // const auto dt = path.size() + 1;  // 1 - to open the door
    const auto dt = DistanceFast(map, cur_door, target) + 1;
    const auto t1 = t_max - dt;
    if (t1 < 0)
      continue; // no sense to go
    long cur_pressure = dt * last_minute;
    Doors open1{open};
    open1.push_back(target);
    cur_pressure += Pressure(map, target, open1, t1);
    pressure = std::max(pressure, cur_pressure);
  }
  return pressure;
}

[[nodiscard]] Map ReadMap(std::string_view file) noexcept {
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
  return map;
}

/*
// open doors in format ,AA, ... ,ZZ,...
[[nodiscard]] long Answer1(std::string_view file) noexcept {
  const auto map = ReadMap(file);
  return Pressure(map, "AA", Doors{}, kT, std::nullopt);
}
*/

} // namespace day16

TEST(AoC22, Day16) {
  const auto answer1 = [](const day16::Map &map) {
    // return day16::Pressure(map, "AA", day16::Doors{}, day16::kT,
    // std::nullopt);
    return day16::Pressure(map, "AA", day16::Doors{}, day16::kT);
  };
  const bool is_test = IsFastOnly(); 
  if (is_test) {
    // std::deque<std::string_view> test_seq = {"DD", "BB", "JJ", "HH", "EE",
    // "CC"};
    const auto test_map = day16::ReadMap("16-sample");
    // EXPECT_EQ(day16::Pressure(test_map, "AA", test_seq, day16::kT), 1651);
    EXPECT_EQ(answer1(test_map), 1651);
  } else {
    const auto map = day16::ReadMap("16");
    EXPECT_EQ(answer1(map), 1775); // takes 56 seconds
  }
}
