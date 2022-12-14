#include "absl/container/flat_hash_map.h"
#include "absl/strings/numbers.h"
#include "common.h"

namespace day14 {
using Point = std::pair<int, int>;
enum class Material { Air, Rock, Sand };
using Map = absl::flat_hash_map<Point, Material>;

// TODO do we need it?
std::pair<Point, Point> MinMax(const Map& map) {
  const Point& p = map.cbegin()->first;
  auto [x0, y0] = p;
  auto [x1, y1] = p;
  for (const auto& [pi, _] : map) {
    const auto [xi, yi] = pi;
    x0 = std::min(x0, xi);
    x1 = std::max(x1, xi);
    y0 = std::min(y0, yi);
    y1 = std::max(y1, yi);
  }
  return {{x0, y0}, {x1, y1}};
}

int Sign(auto x) {
  return (x > 0) ? 1 : ((x < 0) ? -1 : 0);
}  // TODO - move to common.h

Map LoadMap(std::string_view file) {
  const auto data = ReadData(file);
  Map map;
  for (const auto& line : data) {
    if ( line.empty() ) continue;
    const std::vector<std::string_view> points_as_string =
        absl::StrSplit(line, " -> ");
    std::vector<Point> points;
    for (const auto& point_as_string : points_as_string) {
      const std::vector<std::string_view> xy =
          absl::StrSplit(point_as_string, ",");
      int x, y;
      if (absl::SimpleAtoi(xy[0], &x) && absl::SimpleAtoi(xy[1], &y)) {
        points.emplace_back(x, y);
      } else EXPECT_TRUE(false) << "Failed to parse: " << point_as_string;
    }
    std::optional<Point> prev;
    for (const auto& p : points) {
      if (prev.has_value()) {
        const auto [x0, y0] = prev.value();
        const auto [x1, y1] = p;
        const auto [dx, dy] = Point{Sign(x1 - x0), Sign(y1 - y0)};
        const bool is_xi = dx != 0;
        EXPECT_TRUE( is_xi ? dy == 0 : dy != 0 ) << "We move only on one direction at a time " << x0 << "," << y0 << " - " << x1 << "," << y1;
        const int a = is_xi ? x0 : y0;
        const int b = is_xi ? x1 : y1;
        const int d = Sign( b - a );
        for (int i = a; true; i += d ) {
          const Point p = is_xi ? Point{i, y0} : Point{x0, i};
          map.insert({p, Material::Rock});
          if ( i == b ) break;
        }
      }
      prev = p;
    }
  }
  EXPECT_TRUE( map.size() > 10 ) << "Map contains not enough elements";
  return map;
}

Material At(const Map& map, const Point p) noexcept {
  const auto i = map.find(p);
  return i == map.end() ? Material::Air : i->second;
}

std::optional<Point> TraceGrainSand(const Map& map) noexcept {
  const auto [p0, p1] = MinMax(map);
  const auto [_, y_max] = p1;
  int x = 500;
  for (int y = 0; y <= y_max; y++) {
    const int y1 = y + 1;
    const std::array xii = {x, x - 1, x + 1};
    bool is_falling = false;
    for (int xi : xii) {
      const auto m_down = At(map, {xi, y1});
      if (m_down == Material::Air) {
        // we just fall down as we are yet in the air
        x = xi;
        is_falling = true;
        break;
      }
    }

    if (is_falling) continue;  // we continue to fall

    // if we are here it means we have found the bottom line for the grain of
    // sand
    return Point{x, y};
  }
  return std::optional<Point>();  // the grain flow into the abyss
}

size_t CountRestSand(const Map& map) noexcept {
  size_t count = {0};
  // TODO count_if ?
  for (const auto& [_, m] : map) {
    count += m == Material::Sand ? 1 : 0;
  }
  return count;
}
}  // namespace day14

TEST(AoC22, Day14) {
  auto map = day14::LoadMap("14");  // 96,30 -> 496,24 -> 496,30
  while (true) {
    const auto p = TraceGrainSand(map);
    if (!p.has_value()) break;
    map.insert({p.value(), day14::Material::Sand});
  }
  EXPECT_EQ(day14::CountRestSand(map), 779);
}
