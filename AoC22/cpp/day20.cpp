#include "common.h"

namespace day20 {
[[nodiscard]] int Answer1(std::string_view file) noexcept {
  const auto data = ReadData(file);
  std::vector<int> seq;
  seq.reserve(data.size());
  for (const std::string &line : data) {
    if (int value{0}; absl::SimpleAtoi(line, &value)) {
      seq.push_back(value);
    } else {
      EXPECT_TRUE(false) << line;
    }
  }
  const auto n = seq.size();
  // mix
  const auto initial{seq};
  for (int i : initial) {
    if (i == 0)
      continue; // no shift is needed
    auto i0 = std::find(seq.cbegin(), seq.cend(), i);
    const int index0 = i0 - seq.cbegin();
    int index1 = (index0 + i);
    index1 += index1 < 0 ? n : 0;
    index1 %= n;
    const bool is_goes_ahead = index0 < index1;
    // to compensate erased head index
    // index1 -= index0 < index1 ? 1 : 0;
    // index1 += index1 < 0 ? n : 0;

    seq.erase(i0);
    seq.insert(seq.begin() + index1 + (is_goes_ahead && i < 0 ? -1 : 0), i);
  }

  // the grove coordinates
  int sum = {0};
  std::array k_pos_list = {1000, 2000, 3000};
  for (int pos : k_pos_list) {
    auto iter = std::find(seq.cbegin(), seq.cend(), 0);
    for (int i = 0; i < pos; i++) {
      auto next = iter + 1;
      iter = next == seq.cend() ? seq.cbegin() : next;
    }
    sum += *iter;
  }
  return sum;
}
} // namespace day20

TEST(AoC22, Day20) {
  EXPECT_EQ(day20::Answer1("20-sample"), 3);
  EXPECT_EQ(day20::Answer1("20"), 0); //  // -19004 not right
}