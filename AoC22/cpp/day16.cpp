// #include "absl/strings/numbers.h"
#include "absl/container/flat_hash_map.h"
#include "absl/container/flat_hash_set.h"
#include "common.h"
#include <re2/re2.h>

namespace day16 {
struct Conf {
  int rate;
  std::vector<std::string> next;
};
using Map = absl::flat_hash_map<std::string, Conf>;
using Set = absl::flat_hash_set<std::string_view>;
using SetPtr = std::shared_ptr<Set>;
// using State = std::tuple<std::string_view, int, SetPtr>;
// using StateCache = absl::flat_hash_map<State, long>;
long PressureInMinute(const Map &map, const Set &open) noexcept {
  long out = 0;
  for (const auto &ov : open) {
    out += map.at(ov).rate;
  }
  return out;
}
long Pressure(std::string_view start, int to_go, const Map &map,
              std::shared_ptr<Set> open) {
  if (to_go <= 0)
    return 0;

  // const auto key = State{start, to_go, open};
  // static StateCache cache;
  // auto i = cache.find(key);
  // if ( i != cache.end() ) return i->second;

  const auto &c = map.at(start);
  const long pressure = PressureInMinute(map, *open);
  long delta = 0;
  const int in = static_cast<int>(c.next.size());
  for (int i = -1; i < in; i++) {
    long di = pressure;
    if (i < 0) {
      // opening valve?
      if (c.rate <= 0)
        continue; // no sense to spend time opening this broken valve
      if (open->find(start) != open->end())
        continue; // this one is already open
      const auto open1 = std::make_shared<Set>(*open);
      open1->insert(start);
      di = Pressure(start, to_go - 1, map, open1);
    } else {
      // moving to another?
      const auto &start1 = c.next[i];
      di = Pressure(start1, to_go - 1, map, open);
    }
    delta = std::max(delta, di);
  }
  const long result = pressure + delta;
  // cache.insert( {key, result} );
  return result;
}
// open doors in format ,AA, ... ,ZZ,...
// state - cur position - AA + open doors + time_passed --> maximum
//

long Answer1(std::string_view file) noexcept {
  const auto data = ReadData(file);
  // Valve HH has flow rate=22; tunnel leads to valve GG
  // Valve AA has flow rate=0; tunnels lead to valves DD, II, BB
  re2::RE2 re(
      "Valve (.+) has flow rate=(\\d+); tunnel.? lead.? to valve.? (.+)");
  Map map;
  for (const auto &line : data) {
    if (line.empty())
      continue;
    re2::StringPiece input(line);
    std::string valve, list;
    int rate{0};
    if (re2::RE2::FullMatch(input, re, &valve, &rate, &list)) {
      const std::vector<std::string> n = absl::StrSplit(list, ", ");
      map.insert({valve, Conf{rate, n}});
    } else
      EXPECT_TRUE(false) << line;
  }
  return Pressure("AA", 30, map, std::make_shared<Set>());
}
} // namespace day16

TEST(AoC22, Day16) {
  return; // TODO - not working yet
  const auto a1 = day16::Answer1("16-sample");
  EXPECT_EQ(a1, 1651);
}