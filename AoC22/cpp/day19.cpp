#include "absl/container/flat_hash_map.h"
#include "common.h"
#include <re2/re2.h>

namespace day19 {
enum class Material { Ore, Clay, Obsidian, Geode };
struct Blueprint {
  int id;
  int ore_robot_cost_ore;      // produces ore
  int clay_robot_cost_ore;     // produces clay from ore
  int obsidian_robot_cost_ore; // produces obsidian from ore and clay
  int obsidian_robot_cost_clay;
  int geode_robot_cost_ore; // produces geode from ore and obsidian
  int geode_robot_cost_obsidian;
};
using MaterialQtyMap = absl::flat_hash_map<Material, int>; // material, qty
std::vector<Blueprint> ReadBlueprints(std::string_view file) {
  const auto data = ReadData(file);
  re2::RE2 re(
      "Blueprint (\\d+): Each ore robot costs (\\d+) ore. Each clay robot "
      "costs (\\d+) ore. Each obsidian robot costs (\\d+) ore and (\\d+) clay. "
      "Each geode robot costs (\\d+) ore and (\\d+) obsidian.");

  std::vector<Blueprint> result;

  int id;
  int ore_robot_cost_ore;      // produces ore
  int clay_robot_cost_ore;     // produces clay from ore
  int obsidian_robot_cost_ore; // produces obsidian from ore and clay
  int obsidian_robot_cost_clay;
  int geode_robot_cost_ore; // produces geode from ore and obsidian
  int geode_robot_cost_obsidian;

  for (const std::string &line : data) {
    if (line.empty())
      continue;
    re2::StringPiece input(line);
    if (re2::RE2::FullMatch(input, re, &id, &ore_robot_cost_ore,
                            &clay_robot_cost_ore, &obsidian_robot_cost_ore,
                            &obsidian_robot_cost_clay, &geode_robot_cost_ore,
                            &geode_robot_cost_obsidian)) {
      const Blueprint b{id,
                        ore_robot_cost_ore,
                        clay_robot_cost_ore,
                        obsidian_robot_cost_ore,
                        obsidian_robot_cost_clay,
                        geode_robot_cost_ore,
                        geode_robot_cost_obsidian};
      result.emplace_back(b);
    } else {
      EXPECT_TRUE(false) << line;
    }
  }
  return result;
}

constexpr int k_time = 24;

// we initially have 1 ore-collecting robot
const MaterialQtyMap k_start_robots = {{Material::Ore, 1},
                                       {Material::Clay, 0},
                                       {Material::Obsidian, 0},
                                       {Material::Geode, 0}};

int LargestGeodes(const Blueprint &b, int time, MaterialQtyMap robots,
                  MaterialQtyMap materials) noexcept {
  if (time > 0) {
    
  }
  return materials[Material::Geode];
}

int LargestGeodes(const Blueprint &b) noexcept {
  return LargestGeodes(b, k_time, k_start_robots, MaterialQtyMap() );
}

} // namespace day19

TEST(AoC22, Day19) {
  const auto tb = day19::ReadBlueprints("19-sample");
  EXPECT_EQ(tb.size(), 2);

  const auto blueprints = day19::ReadBlueprints("19");
  EXPECT_EQ(blueprints.size(), 30);
  /*
  std::vector<long> sums;
  long sum{0};
  const auto sum_highest_n = [&sums](size_t n) {
    return std::accumulate(sums.rbegin(), sums.rbegin() + n, 0);
  };
  EXPECT_EQ(sum_highest_n(1), 70698);
  EXPECT_EQ(sum_highest_n(3), 206643);
   */
}