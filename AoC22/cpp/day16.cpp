#include <algorithm>
#include <cassert> // assert()
#include <deque>
#include <iostream> // cerr
#include <map>
#include <optional>
#include <queue>
#include <regex>

#include "common-std.h"
#include "day16.h"

namespace day16 {

[[nodiscard]] long PressureInMinute(const Map &map,
                                    const Doors &open) noexcept {
  // We may cache this method as well.
  long out = 0;
  for (const Door &ov : open) {
    out += map.at(ov).rate;
  }
  return out;
}

[[nodiscard]] Doors AllDoors(const Map &map) noexcept {
  Doors doors;
  std::for_each(map.begin(), map.end(),
                [&doors](const auto &pair) { doors.push_back(pair.first); });
  return doors;
}

// Finds the distance but the method is slow.
// It is invoked by DistanceFast only once to determine the distance between
// doors.
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
  using Cache = std::map<Input, int>;
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
    if (std::find(next_doors.begin(), next_doors.end(), to) != next_doors.end())
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
  using Cache = std::map<Input, Doors>;
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
  std::for_each(open.begin(), open.end(), [&doors](const auto &door) {
    doors.erase(std::find(doors.begin(), doors.end(), door));
  });
}

void RemoveNoPressure(Doors &doors, const Map &map) noexcept {
  std::for_each(map.begin(), map.end(), [&doors](const auto &pair) {
    if (pair.second.rate == 0) {
      doors.erase(std::find(doors.begin(), doors.end(), pair.first));
    }
  });
}

// highest rates at the beginning with shortest path to it
void Order(const Map &map, Doors &doors, const Door &from) noexcept {
  std::sort(doors.begin(), doors.end(),
            [&map, &from](const auto &a, const auto &b) {
              const auto vf = [&map, &from](const auto &x) {
                const int d = DistanceFast(map, from, x);
                return map.at(x).rate - d;
              };
              return vf(a) > vf(b);
            });
}

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
const Door kNoDoor = "??";
[[nodiscard]] long Pressure2(const Map &map, State actor_a, State actor_b,
                             Doors open, int t) noexcept {
  if (t < 0)
    return 0;

  const long last_minute = PressureInMinute(map, open);
  if (t == 0)
    // last step
    return last_minute;

  const auto [pos_a, target_a] = actor_a;
  const auto [pos_b, target_b] = actor_b;

  // XXX: this part can be cached one per all the times.
  // 1. get all doors
  Doors doors = AllDoors(map);
  // 2. remove no pressure doors
  RemoveNoPressure(doors, map);

  if (doors.size() == open.size()) {
    // all doors are open
    return last_minute + Pressure2(map, actor_a, actor_b, open, t - 1);
  }

  // 3. remove opened doors
  RemoveOpened(doors, open);
  const size_t doors_count = doors.size();
  Doors targets_a, targets_b;
  if (doors_count == 1) {
    const Door &d = doors[0];
    const int a = SequenceFast(map, pos_a, d).size();
    const int b = SequenceFast(map, pos_b, d).size();
    if (a < b) {
      targets_a.push_back(d);
      targets_b.push_back(kNoDoor);
    } else {
      targets_a.push_back(kNoDoor);
      targets_b.push_back(d);
    }
  } else if (doors_count == 2) {
    // choice of targets is straightforward
    const Door &d0 = doors[0];
    const Door &d1 = doors[1];

    // option 1
    const int a0 = SequenceFast(map, pos_a, d0).size();
    const int b1 = SequenceFast(map, pos_b, d1).size();

    // option 2
    const int a1 = SequenceFast(map, pos_a, d1).size();
    const int b0 = SequenceFast(map, pos_b, d0).size();

    // The longest of the distances is the real critical path.
    if (std::max(a0, b1) < std::max(a1, b0)) {
      // option 1 is better
      targets_a.push_back(d0);
      targets_b.push_back(d1);
    } else {
      targets_a.push_back(d1);
      targets_b.push_back(d0);
    }
  } else {
    // more then 2 doors to choice from
    const auto is_valid_target = [&open](std::optional<Door> target) -> bool {
      return target.has_value() &&
             std::find(open.begin(), open.end(), target.value()) == open.end();
    };

    const auto init_targets =
        [&doors, &is_valid_target](std::optional<Door> target) -> Doors {
      return is_valid_target(target) ? Doors{target.value()}
             : doors.empty()         ? Doors{kNoDoor}
                                     : doors;
    };

    targets_a = init_targets(target_a);
    Order(map, targets_a, pos_a);
    targets_b = init_targets(target_b);
    Order(map, targets_b, pos_b);
  }

  const auto seq = [&map](Door pos, Door target) {
    return target == kNoDoor ? Doors{} : SequenceFast(map, pos, target);
  };

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
          if (std::find(open1.begin(), open1.end(), pos) == open1.end())
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
  std::regex re(
      "Valve (.+) has flow rate=(\\d+); tunnel.? lead.? to valve.? (.+)");
  Map map;
  for (const auto &line : data) {
    std::string valve, list;
    int rate{0};
    std::smatch what;
    // if (re2::RE2::FullMatch(input, re, &valve, &rate, &list)) {
    if (std::regex_match(line, what, re)) {
      valve = what[1];
      rate = stoi(what[2]);
      list = what[3];
      // const std::vector<std::string> n = absl::StrSplit(list, ", ");
      const std::vector<std::string> n = StrSplit(list, ", ");
      map.insert({valve, Conf{rate, n}});
    } else {
      std::cerr << line;
      assert(false);
    }
  }
  return map;
}

[[nodiscard]] long Answer1(const Map &map) noexcept {
  return day16::Pressure(map, kStart, Doors{}, 30);
}

[[nodiscard]] long Answer2(const Map &map) noexcept {
  State start{kStart, std::nullopt};
  return day16::Pressure2(map, start, start, Doors{}, 26);
}

} // namespace day16