#include "common.h"
#include "day19.h"
#include <future> // std::async

TEST(AoC22, Day19) {
  const auto tb = day19::ReadBlueprints("19-sample");
  EXPECT_EQ(tb.size(), 2);

  const auto blueprints = day19::ReadBlueprints("19");
  EXPECT_EQ(blueprints.size(), 30);

  const day19::IdGeodesList test_list = {{1, 9}, {2, 12}};
  EXPECT_EQ(day19::QualityLevel(test_list), 33);

  if (IsFastOnly())
    return; // 733 seconds on local PC
  if (true) {
    EXPECT_EQ(day19::LargestGeodes1(tb[0]), 9);
    EXPECT_EQ(day19::LargestGeodes1(tb[1]), 12);

    // answer 1
    day19::IdGeodesList list;
    for (const auto &b : blueprints) {
      list[b.id] = day19::LargestGeodes1(b);
    }
    EXPECT_EQ(day19::QualityLevel(list), 600);
  }
  if (false) {
    auto a = std::async(std::launch::async, [&blueprints] {
      return day19::LargestGeodes2(blueprints[0]);
    });
    auto b = std::async(std::launch::async, [&blueprints] {
      return day19::LargestGeodes2(blueprints[1]);
    });
    auto c = std::async(std::launch::async, [&blueprints] {
      return day19::LargestGeodes2(blueprints[2]);
    });
    EXPECT_EQ(a.get() * b.get() * c.get(), 0);
  }
}
