#include "common.h"

namespace day20 {
using Int = long long;

struct Node {
  const Int value;
  Node *prev;
  Node *next;
};

void Move(Node *node, bool is_forward) noexcept {
  auto *prev_t0 = node->prev;
  auto *next_t0 = node->next;
  if (is_forward) {
    auto *next2_t0 = next_t0->next;
    // [prev_t0] [next_t0] [node] [next2_t0]
    prev_t0->next = next_t0;
    next_t0->next = node;
    node->next = next2_t0;
    next2_t0->prev = node;
    node->prev = next_t0;
    next_t0->prev = prev_t0;
  } else {
    auto *prev2_t0 = prev_t0->prev;
    // [prev2_t0] [node] [prev_t0] [next_t0]
    prev2_t0->next = node;
    node->next = prev_t0;
    prev_t0->next = next_t0;
    next_t0->prev = prev_t0;
    prev_t0->prev = node;
    node->prev = prev2_t0;
  }
}

constexpr Int kDecryptionKey = 811589153;

[[nodiscard]] Int Answer(std::string_view file, bool is_part1) noexcept {
  const auto data = ReadData(file);
  std::vector<Node> nodes;
  for (const std::string &line : data) {
    if (Int value{0}; absl::SimpleAtoi(line, &value)) {
      if (!is_part1)
        value *= kDecryptionKey;
      Node node{value, nullptr, nullptr};
      nodes.emplace_back(std::move(node));
    } else {
      EXPECT_TRUE(false) << line;
    }
  }
  const Int size = nodes.size();
  // all nodes read, now we may adjust prev and next
  auto &last = nodes.back();
  auto &first = nodes.front();

  auto *prev = &last;
  for (auto &e : nodes) {
    e.prev = prev;
    prev = &e;
  }

  auto *next = &first;
  for (auto &e : nodes | rv::reverse) {
    e.next = next;
    next = &e;
  }

  // mix
  const auto n = is_part1 ? 1 : 10;
  for (int i = 0; i < n; i++) {
    for (auto &node : nodes) {
      const auto is_forward = node.value > 0;
      const Int n = abs(node.value); // % size;
      for (Int i = 0; i < n; i++) {
        Move(&node, is_forward);
      }
    }
  }

  // the grove coordinates
  auto *zero = &first;
  while (zero->value != 0) {
    zero = zero->next;
  }

  Int sum = {0};
  std::array k_pos_list = {1000, 2000, 3000};
  for (int pos : k_pos_list) {
    auto *cur = zero;
    for (int i = 0; i < pos; i++) {
      cur = cur->next;
    }
    sum += cur->value;
  }
  return sum;
}

// old implementation starts

using Seq = std::vector<int>;

/*
void Move( Seq & seq, int old_index, int new_index ) noexcept {
    if ( old_index == new_index ) return;
    const int value = seq.at(old_index);
    seq.insert(seq.begin() + new_index, value);
    seq.erase(seq.begin() + old_index);
}

void Move( Seq & seq, int value ) noexcept {
    const auto i0 = std::find(seq.cbegin(), seq.cend(), value);
    const int index0 = i0 - seq.cbegin();
    int index1 = (index0 + value);
    const auto n = seq.size();
    while ( index1 < 0 ) index1 += n;
    while ( index1 >= n ) index1 -= n;
    if ( index1 < 0 || index1 >= n )
      EXPECT_TRUE(false) << value << " index0 " << index0 << " index1 " <<
index1 << " n " << n; Move(seq, index0, index1);
}
*/

[[nodiscard]] int Normalize(int position, int n) noexcept {
  if (position < 0)
    position += n;
  else if (position >= n)
    position -= n;
  return position;
}

void Move(Seq &seq, int value) noexcept {
  if (value == 0)
    return;
  const auto i0 = std::find(seq.cbegin(), seq.cend(), value);
  const int index0 = i0 - seq.cbegin();
  const auto n = abs(value);
  const auto sz = seq.size();
  const auto d = value > 0 ? 1 : -1;
  auto old_index = index0;
  for (int i = 0; i < n; i++) {
    auto new_index = Normalize(old_index + d, sz);
    if (new_index == 0 && d > 0) {
      seq.pop_back();
      seq.insert(seq.begin(), value);
      old_index = 0;
      new_index = 1;
      // continue;
    }
    std::swap(seq.at(old_index), seq.at(new_index));
    old_index = new_index;
    if (old_index == 0 && d < 0) {
      seq.erase(seq.begin());
      seq.push_back(value);
      old_index = sz - 1;
      // new_index = Normalize(old_index + d, sz);
    }
  }
}

[[nodiscard]] std::string ToStr(const Seq &s) noexcept {
  std::string fmt;
  for (const auto &e : s) {
    fmt += absl::StrCat(e, ",");
  }
  return fmt;
}

void RotateToRight(Seq &seq) noexcept {
  const auto value = seq.back();
  seq.pop_back();
  seq.insert(seq.begin(), value);
}

[[nodiscard]] int Answer1_(std::string_view file) noexcept {
  const auto data = ReadData(file);
  Seq seq;
  seq.reserve(data.size());
  for (const std::string &line : data) {
    if (int value{0}; absl::SimpleAtoi(line, &value)) {
      seq.push_back(value);
    } else {
      EXPECT_TRUE(false) << line;
    }
  }
  // mix
  const auto initial{seq};
  for (int value : initial) {
    Move(seq, value);
    // EXPECT_TRUE(false) << ToStr( seq );
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

void Test(Seq s, int value, Seq e) noexcept {
  const auto s0 = ToStr(s);
  Move(s, value);
  EXPECT_EQ(ToStr(s), ToStr(e)) << "diff for " << s0 << " with value " << value;
}

} // namespace day20

TEST(AoC22, Day20) {

  // day20::Test(day20::Seq{1, 3, 2, 4}, 2, day20::Seq{1, 2, 3, 4});

  EXPECT_EQ(day20::Answer("20-sample", true), 3);
  // return ;
  /*
    day20::Test(day20::Seq{4, 5, 6, 1, 7, 8, 9}, 1,
                day20::Seq{4, 5, 6, 7, 1, 8, 9});
    day20::Test(day20::Seq{4, -2, 5, 6, 7, 8, 9}, -2,
                day20::Seq{4, 5, 6, 7, 8, -2, 9});

    day20::Test(day20::Seq{1, 2, 3, 4}, 2, day20::Seq{1, 3, 4, 2});
    // day20::Test(day20::Seq{1, -1, 3, 4}, -1, day20::Seq{-1, 1, 3, 4});
    day20::Test(day20::Seq{1, 2, -2, -3, 0, 3, 4}, -2,
                day20::Seq{1, 2, -3, 0, 3, 4, -2});
    */
  EXPECT_EQ(day20::Answer("20", true), 7713);
  // 
  if (IsFastOnly())
    return; // TODO no valid solution yet
  EXPECT_EQ(day20::Answer("20", false), 0);
  // -19004 not right -9108 -4389 -5382 4924
}