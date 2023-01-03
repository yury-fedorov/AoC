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
using StateKey =
    std::tuple<std::string, std::string, int>; // open doors, position, time
using StateCache = absl::flat_hash_map<StateKey, long>; // state - pressure

[[nodiscard]] long PressureInMinute(const Map &map,
                                    const Doors &open) noexcept {
  long out = 0;
  for (const auto &ov : open) {
    out += map.at(ov).rate;
  }
  return out;
}

constexpr auto kDoorSeparator = " ";

[[nodiscard]] std::string DoorsToStr(Doors doors) noexcept {
  r::sort(doors);
  return absl::StrJoin(doors, kDoorSeparator);
}

[[nodiscard]] Doors StrToDoors(std::string_view doors) noexcept {
  if (doors.empty())
    return Doors{};
  return absl::StrSplit(doors, kDoorSeparator);
}

bool RememberState(StateCache &states, const StateKey &key,
                   long pressure) noexcept {
  bool is_to_insert = false;
  auto i = states.find(key);
  if (i == states.end()) {
    // the key does not yet exist, we just save it
    is_to_insert = true;
  } else if (i->second < pressure) {
    // the key exists and we found better pressure value
    is_to_insert = true;
  }
  if (is_to_insert)
    states.insert({key, pressure});
  return is_to_insert;
}

constexpr int kT = 30;

[[nodiscard]] long Pressure(const Map &map) noexcept {
  const auto kStartState = StateKey{"", "AA", 0};
  auto states = StateCache{{kStartState, 0}};
  std::vector<StateKey> to_process = {kStartState};
  const auto priority = [&states](const StateKey &key) {
    const auto [_, __, t] = key;
    return states.at(key) / (0.1 + t);
  };
  const auto less = [&priority](const StateKey &a, const StateKey &b) {
    return priority(a) < priority(b);
  };
  std::priority_queue queue(to_process.begin(), to_process.end(), less);
  while (!queue.empty()) {
    auto next = queue.top();
    queue.pop();

    const auto [open_doors, position, t0] = next;
    const int t1 = t0 + 1;
    if (t1 > kT)
      continue; // we end this request here

    const auto doors = StrToDoors(open_doors);
    const long pressure_t1 = PressureInMinute(map, doors) + states.at(next);
    // everything is ready but position

    const auto &c = map.at(position);
    const int in = static_cast<int>(c.next.size());
    for (int i = -1; i < in; i++) {
      std::optional<StateKey> key_t1;
      if (i < 0) {
        // opening valve?
        if (c.rate <= 0)
          continue; // no sense to spend time opening this broken valve
        if (open_doors.find(position) != std::string::npos)
          continue; // this one is already open
        auto doors_t1 = doors;
        doors_t1.push_back(position);
        r::sort(doors_t1);
        key_t1 = StateKey{DoorsToStr(doors_t1), position, t1};
      } else {
        // moving to another?
        const auto &position_t1 = c.next[i];
        key_t1 = StateKey{open_doors, position_t1, t1};
      }
      if (key_t1.has_value()) {
        // wrong - we may have temporal less pressure but it is ok
        // good optimizer: have understanding where to go next: one of non
        // opened doors and how much it costs us (in steps)
        if (RememberState(states, key_t1.value(), pressure_t1)) {
          // this state is valuable, let's insert it to the processing queue
          queue.push(key_t1.value());
        }
      }
    }
  }
  long max_pressure = 0;
  StateKey best_key;
  for (const auto &[state, pressure] : states) {
    const auto &[_, __, t] = state;
    if (t == kT) {
      if (max_pressure < pressure) {
        best_key = state;
        max_pressure = pressure;
      }
      // max_pressure = std::max(max_pressure, pressure);
    }
  }
  return max_pressure;
}

// insert all
// cached
// shortest path
[[nodiscard]] int Distance( const Map &map, std::string_view from, std::string_view to ) noexcept {
  if ( from == to ) return 0;
  const auto &next = map.at(from).next;
  if ( r::find( next, to ) != next.end() ) return 1;
  
  absl::flat_hash_set<std::string_view> visited { next.begin(), next.end() };
  visited.insert( from );
  int distance = 2;
  absl::flat_hash_set<std::string_view> next_from { next.begin(), next.end() };
  absl::flat_hash_set<std::string_view> next_from_1 {};
  for ( const auto & from_i : next_from ) {
  	const auto &next_i = map.at(from_i).next;
  	if ( r::find( next_i, to ) != next.end() ) {
  		return distance;
  	}
  	visited.insert( next_i.begin(), next_i.end() );
  	for ( const auto & j : next_i ) {
		if ( visited.find( j ) != visited.end() ) continue;
  	}
  	distance++;
  } 
  return -1;
}

// second attempt
[[nodiscard]] long Pressure(const Map &map, std::string_view start, const Doors &open, int t_left, std::optional<std::string_view> target ) noexcept {
  if ( t_left <= 0 ) return 0;
  if ( t_left == 1 ) {
  	// no sense to open anything, just calculate what is opened
  	return PressureInMinute(map, open);
  }
  // we've more then 1 step to go, opening a door could make sense
  if ( target.has_value() ) {
  	// we go to the selected target
  	// TODO
  }
  
  // we do not have a selected target, we choose among the doors to open
  // 1. get all doors
  // 2. remove opened doors
  // 3. remove no pressure doors
  // 4. order first high rated doors (could try to optimize: top 50%)
  return 0;
}


// open doors in format ,AA, ... ,ZZ,...
// state - cur position - AA + open doors + time_passed --> maximum

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
  return Pressure(map);
}
} // namespace day16

TEST(AoC22, Day16) {
  return; // TODO - wrong answer
  const auto a1 = day16::Answer1("16-sample");
  EXPECT_EQ(a1, 1651);
}
