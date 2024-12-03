#include "common.h"
#include "day16.h"

TEST(AoC22, Day16) {
  const bool kIsTest = IsFastOnly(); // takes 56 seconds
  const std::string_view kFile = kIsTest ? "16-sample" : "16";
  const auto map = day16::ReadMap(kFile);
  EXPECT_EQ(day16::Answer1(map), kIsTest ? 1651 : 1775);

  // TODO - Day16 Part2 to be solved
  if (IsGreenOnly())
    return;

  EXPECT_EQ(day16::Answer2(map), kIsTest ? 1707 : 0);
}
