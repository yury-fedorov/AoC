#include <future>

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

// we initially have 1 ore-collecting robot
const MaterialQtyMap kStartRobots = {{Material::Ore, 1},
                                     {Material::Clay, 0},
                                     {Material::Obsidian, 0},
                                     {Material::Geode, 0}};

// ore -> universal can be spend between all types of robots
// geode - ore + obsidian ( ore + clay ( ore ) )
// we start with 1 ore robot - 24 ore
// next step ore or clay robot
// next step ore or clay or obsidian robot
// next step ore ore clay ore obsidian or geode robot

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

// helps to understand quickly if something is missing among materials
[[nodiscard]] bool IsMissing(const OreClayObsidian &oco) noexcept {
  const auto [a, b, c] = oco;
  return a != 0 || b != 0 || c != 0;
}

// may I produce this type of robot with given amount of resources?
[[nodiscard]] bool CanProduce(const Blueprint &b,
                              const MaterialQtyMap &materials,
                              Material robot_type) noexcept {
  const auto missing = MissingForRobot(b, materials, robot_type);
  return !IsMissing(missing);
}

// produce the requested type of robot
void Produce(const Blueprint &b, MaterialQtyMap &materials,
             MaterialQtyMap &robots, Material robot_type) noexcept {
  const auto [ore, clay, obsidian] = Cost(b, robot_type);
  materials[Material::Ore] -= ore;
  materials[Material::Clay] -= clay;
  materials[Material::Obsidian] -= obsidian;
  robots[robot_type] += 1;
}

void Collect(const MaterialQtyMap &robots, MaterialQtyMap &materials,
             int times = 1) noexcept {
  for (const auto [type, qty] : robots) {
    materials.at(type) += times * qty;
  }
}

// checks if we have all necessary robot types to produce the requested type of
// robot ever without any further robot construction
[[nodiscard]] bool EnoughRobotTypes(Material robot_type,
                                    const Blueprint &blueprint,
                                    const MaterialQtyMap &robots) noexcept {
  const auto [ore, clay, obsidian] = Cost(blueprint, robot_type);
  if (ore > 0 && robots.at(Material::Ore) == 0) return false;
  if (clay > 0 && robots.at(Material::Clay) == 0) return false;
  if (obsidian > 0 && robots.at(Material::Obsidian) == 0) return false;
  return true;
}

// calculates how much time is requested to produce required type of robot
// without any further robot construction (so just waiting)
[[nodiscard]] std::optional<int> RequiredTime(
    Material robot_type, const Blueprint &blueprint,
    const MaterialQtyMap &robots, const MaterialQtyMap &materials) noexcept {
  if (!EnoughRobotTypes(robot_type, blueprint, robots))
    return std::nullopt;  // we never produce this robot
  const auto oco = MissingForRobot(blueprint, materials, robot_type);
  // if ( !IsMissing(oco) ) return 0; // no time to wait, we may produce
  // immediately, all necessary materials are available
  const auto [ore, clay, obsidian] = oco;  // missing
  const auto get_time = [&robots](int missing, Material material) {
    return static_cast<int>(
        ceil(static_cast<double>(missing) / robots.at(material)));
  };
  const int t1 = get_time(ore, Material::Ore);
  const int t2 = get_time(clay, Material::Clay);
  const int t3 = get_time(obsidian, Material::Obsidian);
  return std::max(std::max(t1, t2), t3);
}

int LargestGeodes(const Blueprint &b, int time, const MaterialQtyMap &robots,
                  const MaterialQtyMap &materials_t0,
                  std::optional<Material> commitment = std::nullopt) noexcept {
  if (time <= 0) return materials_t0.at(Material::Geode);
  auto materials_t1{materials_t0};
  Collect(robots, materials_t1);
  if (time == 1) {
    // no sense to invest anymore, only to wait
    return materials_t1.at(Material::Geode);
  }

  const auto time_1 = time - 1;

  if (commitment.has_value()) {
    // we know what we have to produce next
    const auto robot_type = commitment.value();
    if (CanProduce(b, materials_t0, robot_type)) {
      auto robots_t1{robots};
      Produce(b, materials_t1, robots_t1, robot_type);
      return LargestGeodes(b, time_1, robots_t1, materials_t1, std::nullopt);
    }
    return LargestGeodes(b, time_1, robots, materials_t1, commitment);
  }

  // we do not have any committment, we choose what to do
  constexpr std::array options = {Material::Ore, Material::Clay,
                                  Material::Obsidian, Material::Geode};

  int result = 0;
  for (const auto robot_type : options) {
    // we try to construct a robot for this material
    // we may do it only if:
    // 1) all required types of material are being produced already (robots)
    // 2) we have enough materials of all types for it

    const auto required_time_option =
        RequiredTime(robot_type, b, robots, materials_t0);
    if (!required_time_option.has_value()) continue;  // not able at all
    const int required_time = required_time_option.value();
    if (required_time >= time) continue;  // no sense, too much time

    int result_i;
    if (required_time <= 0) {
      // we produce immediately
      auto robots_t1{robots};
      auto materials_t1i{materials_t1};  // clone
      Produce(b, materials_t1i, robots_t1, robot_type);
      result_i =
          LargestGeodes(b, time_1, robots_t1, materials_t1i, std::nullopt);
    } else {
      // we have to wait
      result_i = LargestGeodes(b, time_1, robots, materials_t1, robot_type);
    }
    result = std::max(result, result_i);
  }

  return result;
}

int LargestGeodes(const Blueprint &b, int time) noexcept {
  const MaterialQtyMap zero_materials = {{Material::Ore, 0},
                                         {Material::Clay, 0},
                                         {Material::Obsidian, 0},
                                         {Material::Geode, 0}};
  return LargestGeodes(b, time, kStartRobots, zero_materials);
}

int LargestGeodes1(const Blueprint &b) noexcept { return LargestGeodes(b, 24); }

int64_t LargestGeodes2(const Blueprint &b) noexcept {
  return LargestGeodes(b, 32);
}

}  // namespace day19

TEST(AoC22, Day19) {
  const auto tb = day19::ReadBlueprints("19-sample");
  EXPECT_EQ(tb.size(), 2);

  const auto blueprints = day19::ReadBlueprints("19");
  EXPECT_EQ(blueprints.size(), 30);

  const day19::IdGeodesList test_list = {{1, 9}, {2, 12}};
  EXPECT_EQ(day19::QualityLevel(test_list), 33);

  // if ( IsFastOnly() ) return; // 103 seconds
  EXPECT_EQ(day19::LargestGeodes1(tb[0]), 9);
  EXPECT_EQ(day19::LargestGeodes1(tb[1]), 12);

  // answer 1
  day19::IdGeodesList list;
  for (const auto &b : blueprints) {
    list[b.id] = day19::LargestGeodes1(b);
  }
  EXPECT_EQ(day19::QualityLevel(list), 600);

  /*  long running
    auto a = std::async(std::launch::async, [&blueprints]{ return
    day19::LargestGeodes2( blueprints[0] ); }); auto b =
    std::async(std::launch::async, [&blueprints]{ return day19::LargestGeodes2(
    blueprints[1] ); }); auto c = std::async(std::launch::async, [&blueprints]{
    return day19::LargestGeodes2( blueprints[2] ); }); EXPECT_EQ( a.get() *
    b.get() * c.get(), 0);
    */
}
