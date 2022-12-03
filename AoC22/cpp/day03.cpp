#include "common.h"

TEST(AoC22, Day03) {
  const auto data = ReadData("03");
  const auto split_in_two = [](std::string_view rucksack) {
    const size_t len = rucksack.length();
    const size_t half = len / 2;
    const auto s = rucksack.data();
    return std::make_pair(std::string_view(s, s + half),
                          std::string_view(s + half, s + len));
  };

  const auto find_same = [](std::string_view a, std::string_view b) {
    for (const char i : b) {
      if (a.find(i) != a.npos)
        return std::optional<char>(i);
    }
    return std::optional<char>();
  };

  const auto calculate_score = [](char c) {
    const bool is_upper = c >= 'A' && c <= 'Z';
    const auto delta = (is_upper ? 'A' : 'a') - 1;
    return c - delta + (is_upper ? 26 : 0);
  };

  long sum{0};
  for (const std::string &rucksack : data) {
    const auto [c0, c1] = split_in_two(rucksack);
    if (c0.empty())
      continue;
    const char same = find_same(c0, c1).value();
    sum += calculate_score(same);
  }

  const std::string t = "vJrwpWtwJgWrhcsFMMfFFhFp";
  const auto [tc0, tc1] = split_in_two(t);
  EXPECT_EQ(tc0, "vJrwpWtwJgWr"); // first
  EXPECT_EQ(tc1, "hcsFMMfFFhFp"); // second
  const char t_same = find_same(tc0, tc1).value();
  EXPECT_EQ(t_same, 'p');
  EXPECT_EQ(calculate_score('b'), 2);
  EXPECT_EQ(calculate_score('A'), 27);

  EXPECT_EQ(sum, 7850);

  const auto find_same_3 = [](std::string_view a, std::string_view b,
                              std::string_view c) {
    for (const char i : a) {
      if (b.find(i) != b.npos && c.find(i) != c.npos)
        return std::optional<char>(i);
    }
    return std::optional<char>();
  };
  long answer2{0};
  std::vector<std::string_view> group;
  for (const std::string &rucksack : data) {
    group.push_back(rucksack);
    if (group.size() == 3) {
      const auto same = find_same_3(group[0], group[1], group[2]);
      group.clear();
      answer2 += calculate_score(same.value());
    }
  }
  EXPECT_EQ(answer2, 2581);
}