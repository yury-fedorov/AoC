#include <algorithm>
#include <assert.h>
#include <catch2/catch.hpp>
#include <climits>
#include <fstream>
#include <iostream>
#include <numeric>
#include <regex>
#include <set>
#include <string_view>

int codeToNumber(const std::string_view code, const char one) {
  int number = 0;
  int position = code.length() - 1;
  for (char chPos : code) {
    if (chPos == one) {
      number |= (1 << position);
    }
    position--;
  }
  return number;
}

int makeSeatId(const std::string_view rowCode,
               const std::string_view columnCode) {
  const auto row = codeToNumber(rowCode, 'B');
  assert(row <= 127 && row >= 0);
  const auto column = codeToNumber(columnCode, 'R');
  assert(column <= 7 and column >= 0);
  return ((row * 8) + column);
}

TEST_CASE("Day05", "[05]") {
  std::ifstream f("05/input.txt");

  auto maxSeatId = INT_MIN;
  auto minSeatId = INT_MAX;
  std::set<int> seats;

  std::string line;
  std::regex re("([BF]{7})([RL]{3})");
  std::smatch what;
  while (getline(f, line)) {
    if (regex_match(line, what, re)) {
      const std::string rowCode = what[1];
      const std::string columnCode = what[2];
      const auto seatId = makeSeatId(rowCode, columnCode);
      maxSeatId = std::max(maxSeatId, seatId);
      minSeatId = std::min(minSeatId, seatId);
      seats.insert(seatId);
    } else {
      std::cerr << "Unexpected line: " << line << std::endl;
    }
  }

  REQUIRE(998 == maxSeatId);

  for (int id = minSeatId; id < maxSeatId; id++) {
    if (seats.find(id) == seats.end()) {
      REQUIRE(676 == id);
      return;
    }
  }
  FAIL();
}
