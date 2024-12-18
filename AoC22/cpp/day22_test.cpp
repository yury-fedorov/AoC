#include "common.h"
#include "day22.h"

TEST(AoC22, Day22) {
  EXPECT_EQ(day22::FinalPassword(6, 8, day22::Direction::kRight), 6032);
  constexpr std::string_view kSample = "22-sample";
  EXPECT_EQ(day22::Answer1(kSample), 6032);
  constexpr std::string_view kFile = "22";
  EXPECT_EQ(day22::Answer1(kFile), 27436);
  EXPECT_EQ(day22::Answer2(kFile), 15426);
}
