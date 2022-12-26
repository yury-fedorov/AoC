#include "common.h"

namespace day20 {
using Int = int64_t;

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

constexpr Int kDecryptionKey = { 811589153 };

[[nodiscard]] Int Answer(std::string_view file, bool is_part1) noexcept {
  const auto data = ReadData(file);
  std::vector<Node> nodes;
  for (const std::string &line : data) {
    if (int value{0}; absl::SimpleAtoi(line, &value)) {
      const Int long_value = is_part1 ? value : ( value * kDecryptionKey );
      Node node{long_value, nullptr, nullptr};
      nodes.emplace_back(std::move(node));
    } else {
      EXPECT_TRUE(false) << line;
    }
  }
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
  const Int modulo = static_cast<Int>( nodes.size() ) - 1;
  const auto n = is_part1 ? 1 : 10;
  for (int i = 0; i < n; i++) {
    for (auto &node : nodes) {
      const auto is_forward = node.value > 0;
      const Int n = abs(node.value) % modulo;
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

} // namespace day20

TEST(AoC22, Day20) {
  const auto answer1 = [](std::string_view file) { return day20::Answer(file, true); };
  const auto answer2 = [](std::string_view file) { return day20::Answer(file, false); };
  EXPECT_EQ(answer1("20-sample"), 3);
  EXPECT_EQ(answer1("20"), 7713);
  EXPECT_EQ(answer2("20-sample"), 1623178306);
  EXPECT_EQ(answer2("20"), 1664569352803);
}