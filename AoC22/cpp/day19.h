#ifndef AOC22_DAY19_H_
#define AOC22_DAY19_H_

#include <array>
#include <map>
#include <tuple>

namespace day19 {

enum class Material : int { Ore = 0, Clay, Obsidian, Geode };
struct Blueprint {
  int id;
  int ore_robot_cost_ore;      // produces ore
  int clay_robot_cost_ore;     // produces clay from ore
  int obsidian_robot_cost_ore; // produces obsidian from ore and clay
  int obsidian_robot_cost_clay;
  int geode_robot_cost_ore; // produces geode from ore and obsidian
  int geode_robot_cost_obsidian;
};
// using MaterialQtyMap = absl::flat_hash_map<Material, int>;  // material, qty
using MaterialQtyMap = std::array<int, 4>; // material, qty
using OreClayObsidian = std::tuple<int, int, int>;
using IdGeodesList = std::map<int, int>;

const static inline MaterialQtyMap zero_materials = {0, 0, 0, 0};
const static inline std::array kMaterials = {
    Material::Geode, Material::Obsidian, Material::Clay, Material::Ore};

std::vector<Blueprint> ReadBlueprints(std::string_view file);

[[nodiscard]] int64_t QualityLevel(const IdGeodesList &list) noexcept;

int LargestGeodes1(const Blueprint &b) noexcept;
int64_t LargestGeodes2(const Blueprint &b) noexcept;

} // namespace day19

#endif // AOC22_DAY19_H_