#include <algorithm>
#include <catch2/catch.hpp>
#include <fstream>
#include <iostream>
#include <vector>

using namespace std;

typedef long long lint;

bool isSum(const vector<lint> &v, lint n) {
  auto rit = v.rbegin();
  std::advance(rit, 25);
  vector<lint> v1(v.rbegin(), rit);
  assert(v1.size() == 25);
  sort(v1.begin(), v1.end());
  const auto min = *v1.begin();
  const auto max = *v1.rbegin();
  for (auto r : v1) {
    const auto d = n - r;
    if (d <= 0 || d < min || d > max || d == r)
      continue;
    if (binary_search(v1.begin(), v1.end(), d))
      return true;
  }
  return false;
}

TEST_CASE("Day09", "[09]") {
  vector<lint> numbers;

  lint answer1 = 0;

  ifstream f("09/input.txt");
  string line;
  while (getline(f, line)) {
    const auto n = stoll(line);
    if (numbers.size() >= 25) {
      if (!isSum(numbers, n)) {
        answer1 = n;
        REQUIRE(29221323 == n);
      }
    }
    numbers.push_back(n);
  }

  const auto n = numbers.size();
  for (size_t i = 0; i < n; i++) {
    lint sum = answer1;
    for (size_t j = i; j < n; j++) {
      sum -= numbers[j];
      if (sum == 0 && (i < j)) { // at least 2 numbers
        // found
        const auto [mi, ma] =
            minmax_element(numbers.begin() + i, numbers.begin() + j + 1);
        REQUIRE(4389369 == (*mi + *ma));
        return;
      } else if (sum < 0)
        break;
    }
  }
  FAIL();
}
