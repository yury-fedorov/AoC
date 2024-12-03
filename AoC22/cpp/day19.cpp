#include <future>

#include "common-std.h"
#include "day19.h"
#include <cassert>
#include <cmath>
#include <optional>
#include <regex>

namespace day19 {

[[nodiscard]] inline int ToIndex(Material material) noexcept {
  const int index{static_cast<int>(material)};
  assert(index >= 0 && index <= 3); // << "Bad conversion: " << index;
  return index;
}

[[nodiscard]] inline int At(const MaterialQtyMap &map,
                            Material material) noexcept {
  return map.at(ToIndex(material));
}

[[nodiscard]] inline int &At(MaterialQtyMap &map, Material material) noexcept {
  return map.at(ToIndex(material));
}

[[nodiscard]] int64_t QualityLevel(const IdGeodesList &list) noexcept {
  int64_t sum{0};
  for (const auto [id, geodes] : list) {
    sum += id * geodes;
  }
  return sum;
}

std::vector<Blueprint> ReadBlueprints(std::string_view file) {
  const auto data = ReadData(file);
  std::regex re(
      "Blueprint (\\d+): Each ore robot costs (\\d+) ore. Each clay robot "
      "costs (\\d+) ore. Each obsidian robot costs (\\d+) ore and (\\d+) clay. "
      "Each geode robot costs (\\d+) ore and (\\d+) obsidian.");

  std::vector<Blueprint> result;

  for (const std::string &line : data) {
    std::smatch what;
    if (std::regex_match(line, what, re)) {
      const int id = stoi(what[1]);
      const int ore_robot_cost_ore = stoi(what[2]);  // produces ore
      const int clay_robot_cost_ore = stoi(what[3]); // produces clay from ore
      const int obsidian_robot_cost_ore =
          stoi(what[4]); // produces obsidian from ore and clay
      const int obsidian_robot_cost_clay = stoi(what[5]);
      const int geode_robot_cost_ore =
          stoi(what[6]); // produces geode from ore and obsidian
      const int geode_robot_cost_obsidian = stoi(what[7]);

      const Blueprint b{id,
                        ore_robot_cost_ore,
                        clay_robot_cost_ore,
                        obsidian_robot_cost_ore,
                        obsidian_robot_cost_clay,
                        geode_robot_cost_ore,
                        geode_robot_cost_obsidian};
      result.emplace_back(b);
    } else {
      assert(false); // << line;
    }
  }
  return result;
}

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
  const int aa = At(materials, Material::Ore);
  const int ab = At(materials, Material::Clay);
  const int ac = At(materials, Material::Obsidian);
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
  At(materials, Material::Ore) -= ore;
  At(materials, Material::Clay) -= clay;
  At(materials, Material::Obsidian) -= obsidian;
  At(robots, robot_type) += 1;
}

void Collect(const MaterialQtyMap &robots, MaterialQtyMap &materials,
             int times = 1) noexcept {
  for (const auto type : kMaterials) {
    const auto qty = At(robots, type);
    At(materials, type) += times * qty;
  }
}

// checks if we have all necessary robot types to produce the requested type of
// robot ever without any further robot construction
[[nodiscard]] bool EnoughRobotTypes(Material robot_type,
                                    const Blueprint &blueprint,
                                    const MaterialQtyMap &robots) noexcept {
  const auto [ore, clay, obsidian] = Cost(blueprint, robot_type);
  if (ore > 0 && At(robots, Material::Ore) == 0)
    return false;
  if (clay > 0 && At(robots, Material::Clay) == 0)
    return false;
  if (obsidian > 0 && At(robots, Material::Obsidian) == 0)
    return false;
  return true;
}

// calculates how much time is requested to produce required type of robot
// without any further robot construction (so just waiting)
[[nodiscard]] std::optional<int>
RequiredTime(Material robot_type, const Blueprint &blueprint,
             const MaterialQtyMap &robots,
             const MaterialQtyMap &materials) noexcept {
  if (!EnoughRobotTypes(robot_type, blueprint, robots))
    return std::nullopt; // we never produce this robot
  const auto oco = MissingForRobot(blueprint, materials, robot_type);
  // if ( !IsMissing(oco) ) return 0; // no time to wait, we may produce
  // immediately, all necessary materials are available
  const auto [ore, clay, obsidian] = oco; // missing
  const auto get_time = [&robots](int missing, Material material) {
    return static_cast<int>(
        ceil(static_cast<double>(missing) / At(robots, material)));
  };
  const int t1 = get_time(ore, Material::Ore);
  const int t2 = get_time(clay, Material::Clay);
  const int t3 = get_time(obsidian, Material::Obsidian);
  return std::max(std::max(t1, t2), t3);
}

/*
OreClayObsidian OneGeode( const Blueprint &b ) noexcept {
        int ore = b.geode_robot_cost_ore;
        const int obsidian = b.geode_robot_cost_obsidian;
        const int clay = obsidian * b.obsidian_robot_cost_clay;
        ore += obsidian * b.obsidian_robot_cost_ore;
        ore += clay * b.clay_robot_cost_ore;
        return ore; // amount of ore to produce 1 geode
}
*/

int LargestGeodes(const Blueprint &b, int time, const MaterialQtyMap &robots,
                  const MaterialQtyMap &materials_t0,
                  std::optional<Material> commitment = std::nullopt) noexcept {
  if (time <= 0)
    return At(materials_t0, Material::Geode);
  auto materials_t1{materials_t0};
  Collect(robots, materials_t1);
  if (time == 1) {
    // no sense to invest anymore, only to wait
    return At(materials_t1, Material::Geode);
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
  const std::array options = kMaterials;
  /*
  constexpr std::array kGeode = {Material::Geode};
  constexpr std::array kObsidian = {Material::Obsidian};
  // we do not have any committment, we choose what to do
  absl::Span<const Material> options = kMaterials;
  if (time == 2) {
    // has sense to invest only in
    options = kGeode;
  }
  */

  int result = 0;
  for (const auto robot_type : options) {
    // we try to construct a robot for this material
    // we may do it only if:
    // 1) all required types of material are being produced already (robots)
    // 2) we have enough materials of all types for it

    const auto required_time_option =
        RequiredTime(robot_type, b, robots, materials_t0);
    if (!required_time_option.has_value())
      continue; // not able at all
    const int required_time = required_time_option.value();
    if (required_time >= time)
      continue; // no sense, too much time

    int result_i;
    if (required_time <= 0) {
      // we produce immediately
      auto robots_t1{robots};
      auto materials_t1i{materials_t1}; // clone
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
  // we initially have 1 ore-collecting robot
  constexpr MaterialQtyMap kStartRobots = {1, 0, 0, 0};
  return LargestGeodes(b, time, kStartRobots, zero_materials);
}

int LargestGeodes1(const Blueprint &b) noexcept { return LargestGeodes(b, 24); }

int64_t LargestGeodes2(const Blueprint &b) noexcept {
  return LargestGeodes(b, 32);
}

} // namespace day19
