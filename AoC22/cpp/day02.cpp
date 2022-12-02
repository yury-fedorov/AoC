#include "common.h"

TEST(AoC22, Day02) {
  constexpr auto ROCK = 'A';
  constexpr auto PAPER = 'B';
  constexpr auto SCISSORS = 'C';
  const auto data = ReadData("02");
  long answer1{0};
  long answer2{0};

  // a - opponent, b - own
  const auto round_score = [](char a, char b) {
    const int shape_score = 1 + (b - ROCK);
    const bool is_draw = a == b;
    if (is_draw)
      return shape_score + 3;
    // rock defeats scissors
    const bool is_won = (b == ROCK && a == SCISSORS)
                        // scissors defeats paper
                        || (b == SCISSORS && a == PAPER)
                        // paper defeats rock
                        || (b == PAPER && a == ROCK);
    return shape_score + (is_won ? 6 : 0);
  };

  const auto round_score1 = [&round_score](char a, char xyz) {
    const char b = xyz - 'X' + ROCK; // normalized to A, B, C
    return round_score(a, b);
  };

  // part 2: X - need to loose, Y - need to draw, Z - need to win
  const auto round_score2 = [&round_score](char a, char xyz) {
    std::array scores = {round_score(a, ROCK), round_score(a, SCISSORS),
                         round_score(a, PAPER)};
    std::ranges::sort(scores);
    return scores.at(xyz - 'X');
  };

  for (const std::string &line : data) {
    if (line.length() < 2)
      continue;
    const char opponent = line.at(0);
    const char xyz = line.at(2);
    answer1 += round_score1(opponent, xyz);
    answer2 += round_score2(opponent, xyz);
  }

  EXPECT_EQ(round_score1('A', 'Y'), 8);
  EXPECT_EQ(answer1, 15523);
  EXPECT_EQ(answer2, 15702);
}