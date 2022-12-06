#include "common.h"
#include <set>

TEST(AoC22, Day06) {
  const auto data = ReadData("06")[0];
  constexpr auto sequence_length_1 = 4;
  const auto is_unique = [](std::string_view data, size_t sequence_length,
                            size_t start) {
    const auto b = data.cbegin() + start;
    const auto e = b + sequence_length;
    std::set<char> s(b, e);
    return s.size() == sequence_length;
  };
  const auto solution = [&is_unique] (std::string_view data,
                                     size_t sequence_length) {
    const auto l = data.length() - sequence_length + 1;
    for (auto i = 0; i < l; i++) {
      if (is_unique(data, sequence_length, i))
        return std::optional<size_t>(i + sequence_length);
    }
    return std::optional<size_t>();
  };

  constexpr auto sequence_length_2 = 14;
  constexpr auto test = "mjqjpqmgbljsphdztnvjfqwrcgsmlb";

  EXPECT_EQ(solution(test, sequence_length_1), 7);
  EXPECT_EQ(solution(data, sequence_length_1), 1651); // part 1

  EXPECT_EQ(solution(test, sequence_length_2), 19);
  EXPECT_EQ(solution(data, sequence_length_2), 3837); // part 2
}