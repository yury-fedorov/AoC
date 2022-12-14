#include "common.h"

namespace day13 {

std::vector<std::string_view> SplitList(std::string_view list) noexcept {
  return {};  // TODO implement
}

bool IsValue(std::string_view text) noexcept {
  return text.find(',') < 0;  // TODO - cover with tests
}

// in all cases highest level is always a list
bool IsRightOrder(std::string_view a, std::string_view b) noexcept {
  const auto al = SplitList(a);
  const auto bl = SplitList(b);
  const auto as = al.size();
  const auto bs = bl.size();
  // If the right list runs out of items first, the inputs are not in the right
  // order.
  if (bs < as) return false;  // right list is shorter
  return false;               // TODO implement
}

int Answer1(std::string_view file) noexcept {
  const auto data = ReadData(file);
  int answer1 = 0;
  int index = 0;
  for (int i = 0; i < data.size() - 1; i++) {
    const auto& a = data[i];
    const auto& b = data[i + 1];
    if (!a.empty() && !b.empty()) {
      index++;
      answer1 += IsRightOrder(a, b) ? index : 0;
    }
  }
  return answer1;
}

}  // namespace day13

TEST(AoC22, Day13) {
  const auto answer1 = day13::Answer1("13-sample");
  EXPECT_EQ(answer1, 13);
}
