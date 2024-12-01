#include <assert.h>
#include <catch2/catch.hpp>
#include <fstream>
#include <iostream>
#include <map>
#include <optional>
#include <vector>

// 3 3 - no alternative
// 3 1 3 - no alternative
// 3 1 1 3 - and 3 2 3 (2)
// 3 1 1 1 3 - and 3 3 3, 3 2 1 3, 3 1 2 3 (4)
// 3 1 1 1 1 3 - and 3 3 1 3, 3 1 3 3, 3 2 2 3, 3 1 2 1 3, 3 2 1 1 3, 3 1 1 2 3
// (7)
std::optional<int> options(int digit, int length) {
  assert(length > 0);
  if (digit == 3)
    return 1;
  assert(digit == 1);
  switch (length) {
  case 1:
    return 1;
  case 2:
    return 2;
  case 3:
    return 4;
  case 4:
    return 7;
  }
  std::cerr << length << std::endl;
  return std::nullopt;
}

TEST_CASE("Day10", "[10]") {
  std::ifstream f("10/input.txt");
  std::vector<int> adapters;
  std::string line;
  while (getline(f, line)) {
    adapters.push_back(stoi(line));
  }
  const auto builtin = *max_element(adapters.begin(), adapters.end()) + 3;
  adapters.push_back(0);
  adapters.push_back(builtin);
  const auto n = adapters.size();
  sort(adapters.begin(), adapters.end());

  std::map<int, int> differences;
  std::vector<int> seq;
  for (size_t i = 1; i < n; i++) {
    const auto diff = adapters[i] - adapters[i - 1];
    differences[diff]++;
    seq.push_back(diff);
  }
  REQUIRE(1914 == (differences[1] * differences[3]));

  std::vector<std::pair<int, int>> grseq;
  int prev = 0;
  int length = 0;
  for (const int se : seq) {
    if (se == prev) {
      length++;
    } else {
      if (length > 0) {
        grseq.emplace_back(prev, length);
      }
      prev = se;
      length = 1;
    }
  }
  grseq.emplace_back(prev, length);

  long long combinations = 1;
  for (const auto &p : grseq) {
    combinations *= options(p.first, p.second).value();
  }

  REQUIRE(9256148959232 == combinations);
}
