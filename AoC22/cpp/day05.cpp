#include "absl/strings/ascii.h"
#include "common.h"

namespace day05 {
std::string solution(const std::vector<std::string> &data,
                     const bool is_part1) {
  std::vector<std::vector<char>> stacks;
  bool is_initialization = true;
  for (const std::string &line : data) {
    if (is_initialization) {
      // we read the crates on top of the stacks
      for (size_t i = 1; i < line.length(); i += 4) {
        const size_t stack = i / 4;
        const size_t min_size = stack + 1;
        if (stacks.size() < min_size)
          stacks.resize(min_size);
        const char crate = line[i];
        if (absl::ascii_isalpha(crate)) {
          // crate found
          auto &s = stacks[stack];
          s.insert(s.begin(), crate);
        } else if (absl::ascii_isdigit(crate)) {
          // this is the last line of initialization
          is_initialization = false;
        }
      }
    } else {
      // follow the instructions (manipulation with the stacks)
      int amount, from, to;
      constexpr auto kFormat = "move %d from %d to %d";
      if (EOF == sscanf(line.c_str(), kFormat, &amount, &from, &to))
        continue;
      auto &stack_from = stacks[from - 1];
      auto &stack_to = stacks[to - 1];
      const long shift = stack_from.size() - amount;
      for (; amount > 0; amount--) {
        if (is_part1) {
          stack_to.push_back(stack_from.back());
          stack_from.pop_back();
        } else {
          stack_to.push_back(stack_from[shift]);
          stack_from.erase(stack_from.cbegin() + shift);
        }
      }
    }
  }
  std::string answer;
  for (const auto &s : stacks) {
    answer += s.empty() ? '?' : s.back();
  }
  return answer;
}
} // namespace day05

TEST(AoC22, Day05) {
  const auto data = ReadData("05");
  EXPECT_EQ(day05::solution(data, true), "ZWHVFWQWW");
  EXPECT_EQ(day05::solution(data, false), "HZFZCCWWV");
}