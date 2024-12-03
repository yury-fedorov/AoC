#include "common.h"
#include "day22.h"

TEST(AoC22, Day22) {
  EXPECT_EQ(day22::FinalPassword(6, 8, day22::Direction::kRight), 6032);
  const auto test = day22::Solution("22-sample");
  EXPECT_EQ(test[0], 6032);
  const auto answers = day22::Solution("22");
  EXPECT_EQ(answers[0], 27436);

  // TODO - Day22 Part2 to be solved
  if (IsGreenOnly())
    return;

  EXPECT_EQ(test[1], 5031);
  EXPECT_EQ(answers[1], 0);
}
