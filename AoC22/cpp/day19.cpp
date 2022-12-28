#include "absl/container/flat_hash_map.h"
#include "common.h"
#include "re2/re2.h"

namespace day19 {
enum class Material { Ore, Clay, Obsidian, Geode };
struct Blueprint {
  int id;
  int ore_robot_cost_ore;       // produces ore
  int clay_robot_cost_ore;      // produces clay from ore
  int obsidian_robot_cost_ore;  // produces obsidian from ore and clay
  int obsidian_robot_cost_clay;
  int geode_robot_cost_ore;  // produces geode from ore and obsidian
  int geode_robot_cost_obsidian;
};
using MaterialQtyMap = absl::flat_hash_map<Material, int>;  // material, qty
using OreClayObsidian = std::tuple<int, int, int>;
using IdGeodesList = absl::flat_hash_map<int, int>;

[[nodiscard]] int64_t QualityLevel(const IdGeodesList &list) noexcept {
  int64_t sum{0};
  for (const auto [id, geodes] : list) {
    sum += id * geodes;
  }
  return sum;
}

std::vector<Blueprint> ReadBlueprints(std::string_view file) {
  const auto data = ReadData(file);
  re2::RE2 re(
      "Blueprint (\\d+): Each ore robot costs (\\d+) ore. Each clay robot "
      "costs (\\d+) ore. Each obsidian robot costs (\\d+) ore and (\\d+) clay. "
      "Each geode robot costs (\\d+) ore and (\\d+) obsidian.");

  std::vector<Blueprint> result;

  int id;
  int ore_robot_cost_ore;       // produces ore
  int clay_robot_cost_ore;      // produces clay from ore
  int obsidian_robot_cost_ore;  // produces obsidian from ore and clay
  int obsidian_robot_cost_clay;
  int geode_robot_cost_ore;  // produces geode from ore and obsidian
  int geode_robot_cost_obsidian;

  for (const std::string &line : data) {
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

[[nodiscard]] OreClayObsidian Cost(const Blueprint &b,
                                   Material robot_type) noexcept {
  int requires_ore{0};
  int requires_clay{0};
  int requires_obsidian{0};
  switch (robot_type) {
    case Material::Ore:
      requires_ore = b.ore_robot_cost_ore;
      break;
    case Material::Clay:
      requires_ore = b.clay_robot_cost_ore;
      break;
    case Material::Obsidian:
      requires_ore = b.obsidian_robot_cost_ore;
      requires_clay = b.obsidian_robot_cost_clay;
      break;
    case Material::Geode:
      requires_ore = b.geode_robot_cost_ore;
      requires_obsidian = b.geode_robot_cost_obsidian;
      break;
  }
  return {requires_ore, requires_clay, requires_obsidian};
}

[[nodiscard]] OreClayObsidian MissingForRobot(const Blueprint &blueprint,
                                              const MaterialQtyMap &materials,
                                              Material robot_type) noexcept {
  const auto [a, b, c] = Cost(blueprint, robot_type);
  const int aa = materials.at(Material::Ore);
  const int ab = materials.at(Material::Clay);
  const int ac = materials.at(Material::Obsidian);
  const auto missing = [](int required, int available) {
    const auto delta = available - required;
    return delta < 0 ? -delta : 0;
  };
  return {missing(a, aa), missing(b, ab), missing(c, ac)};
}

[[nodiscard]] bool IsMissing(const OreClayObsidian &oco) noexcept {
  const auto [a, b, c] = oco;
  return a != 0 || b != 0 || c != 0;
}

[[nodiscard]] bool CanProduce(const Blueprint &b,
                              const MaterialQtyMap &materials,
                              Material robot_type) noexcept {
  // const auto [ore, clay, obsidian] = Cost(b, robot_type);
  const auto missing = MissingForRobot(b, materials, robot_type);
  return !IsMissing(missing);
}

void Produce(const Blueprint &b, MaterialQtyMap &materials,
             MaterialQtyMap &robots, Material robot_type) noexcept {
  const auto [ore, clay, obsidian] = Cost(b, robot_type);
  materials[Material::Ore] -= ore;
  materials[Material::Clay] -= clay;
  materials[Material::Obsidian] -= obsidian;
  robots[robot_type] += 1;
}

int LargestGeodes(const Blueprint &b, int time, MaterialQtyMap robots,
                  MaterialQtyMap materials) noexcept {
  if (time > 0) {
    std::vector<std::optional<Material>> options = {
        std::optional<Material>(), std::optional<Material>(Material::Ore),
        std::optional<Material>(Material::Clay),
        std::optional<Material>(Material::Obsidian),
        std::optional<Material>(Material::Geode)};
    int result = 0;
    for (const auto &o : options) {
      if (o.has_value()) {
        // we try to construct a robot for this material
      } else {
        // we try not to do anything and just wait
        // TODO how we pass back updated amount of material and robots?
      }
    }
  }
  return materials[Material::Geode];
}

int LargestGeodes(const Blueprint &b) noexcept {
  return LargestGeodes(b, k_time, k_start_robots, MaterialQtyMap());
}

}  // namespace day19

TEST(AoC22, Day19) {
  const auto tb = day19::ReadBlueprints("19-sample");
  EXPECT_EQ(tb.size(), 2);

  const auto blueprints = day19::ReadBlueprints("19");
  EXPECT_EQ(blueprints.size(), 30);

  const day19::IdGeodesList test_list = {{1, 9}, {2, 12}};
  EXPECT_EQ(day19::QualityLevel(test_list), 33);
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
