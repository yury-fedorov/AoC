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
using Door = std::string_view;
using Doors = std::vector<Door>;
using DoorSet = absl::flat_hash_set<Door>;

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
  absl::c_for_each(map,
                   [&doors](const auto &pair) { doors.push_back(pair.first); });
  return doors;
}

[[nodiscard]] int Distance(const Map &map, Door from, Door to) noexcept {
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
[[nodiscard]] int DistanceFast(const Map &map, Door from, Door to) noexcept {
  using Input = std::pair<Door, Door>;
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

[[nodiscard]] Doors Sequence(const Map &map, Door from, Door to,
                             int depth) noexcept {
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

[[nodiscard]] Doors SequenceFast(const Map &map, Door from, Door to) noexcept {
  using Input = std::pair<Door, Door>;
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

[[nodiscard]] long Pressure(const Map &map, Door cur_door, Doors open,
                            int t_max) noexcept {
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

using State = std::pair<Door, std::optional<Door>>; // position, target
constexpr Door kNoDoor = "??";
[[nodiscard]] long Pressure2(const Map &map, State actor_a, State actor_b,
                             Doors open, int t) noexcept {
  if (t < 0)
    return 0;
  const auto last_minute = PressureInMinute(map, open);
  const auto [pos_a, target_a] = actor_a;
  const auto [pos_b, target_b] = actor_b;

  // 1. get all doors
  Doors doors = AllDoors(map);
  // 2. remove opened doors
  RemoveOpened(doors, open);
  // 3. remove no pressure doors
  RemoveNoPressure(doors, map);

  // no sense to provide among options doors we already go
  // XXX - really? if no more doors are available, then target of other actor is
  // also good
  Doors targets;
  if (target_a.has_value())
    targets.push_back(target_a.value());
  if (target_b.has_value())
    targets.push_back(target_b.value());
  RemoveOpened(doors, targets);

  const auto init_targets = [&doors](std::optional<Door> target) -> Doors {
    return target.has_value() ? Doors{target.value()}
           : doors.empty()    ? Doors{kNoDoor}
                              : doors;
  };

  const auto seq = [&map](Door pos, Door target) {
    return target == kNoDoor ? Doors{} : SequenceFast(map, pos, target);
  };

  const Doors targets_a = init_targets(target_a);
  const Doors targets_b = init_targets(target_b);

  long pressure{t * last_minute};
  for (const auto &target_ai : targets_a) {
    for (const auto &target_bi : targets_b) {
      // no sense to follow for both the same door
      if (target_ai == target_bi && target_ai != kNoDoor)
        continue;
      Doors open1{open};
      const auto step = [&open1, &seq](Door pos, Door target) -> State {
        const Doors &path = seq(pos, target);
        if (path.empty()) {
          // we do not need to go anywhere but just open
          if (absl::c_find(open1, pos) == open1.end())
            open1.push_back(pos);
          return State{pos, std::nullopt};
        }
        // we need to make the next step
        return State{path.front(), target};
      };
      const auto actor_a1 = step(pos_a, target_ai);
      const auto actor_b1 = step(pos_b, target_bi);
      const auto cur_pressure =
          last_minute + Pressure2(map, actor_a1, actor_b1, open1, t - 1);
      pressure = std::max(pressure, cur_pressure);
    }
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

} // namespace day16

constexpr day16::Door kStart = "AA";

TEST(AoC22, Day16) {
  const auto answer1 = [](const day16::Map &map) {
    // return day16::Pressure(map, "AA", day16::Doors{}, day16::kT,
    // std::nullopt);
    return day16::Pressure(map, kStart, day16::Doors{}, 30);
  };
  const auto answer2 = [](const day16::Map &map) {
    // return day16::Pressure(map, "AA", day16::Doors{}, day16::kT,
    // std::nullopt);
    day16::State start{kStart, std::nullopt};
    return day16::Pressure2(map, start, start, day16::Doors{}, 24);
  };
  const bool is_test = IsFastOnly(); // takes 56 seconds
  const auto file = is_test ? "16-sample" : "16";
  const auto map = day16::ReadMap(file);
  EXPECT_EQ(answer1(map), is_test ? 1651 : 1775);

  // TODO - Day16 Part2 to be solved
  if (IsGreenOnly())
    return;

  EXPECT_EQ(answer2(map), is_test ? 1707 : 0);
}
