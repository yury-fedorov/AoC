#include "common.h"

namespace day24 {

using Map = std::vector<std::string>;
[[nodiscard]] size_t Answer1(const Map &map) noexcept {
  return 0; // TODO
}

} // namespace day24

TEST(AoC22, Day24) {
  const day24::Map map = ReadData("24");
  EXPECT_EQ(day24::Answer1(map), 0);
}
