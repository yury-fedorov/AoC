#include <algorithm>
#include <catch2/catch.hpp>
#include <fstream>
#include <iostream>
#include <regex>

using namespace std;

auto answer(const bool isFirstAnswer) {
  auto validPasswordCount = 0;

  ifstream f("02/input.txt");
  regex re("(\\d+)-(\\d+) ([a-z]): ([a-z]+)");
  string line;
  while (getline(f, line)) {
    smatch what;
    if (regex_match(line, what, re)) {
      const auto min = stoi(what[1]);
      const auto max = stoi(what[2]);
      const char symbol = what[3].str()[0];
      const string pwd = what[4];
      if (isFirstAnswer) {
        const auto count = count_if(cbegin(pwd), cend(pwd),
                                    [symbol](char ch) { return symbol == ch; });
        validPasswordCount += count >= min && count <= max;
      } else {
        const auto match_at = [symbol, pwd](int oneBasedIndex) {
          return symbol == pwd[oneBasedIndex - 1];
        };
        validPasswordCount += (match_at(min) + match_at(max)) == 1;
      }
    } else {
      cerr << "Unexpected line: " << line << endl;
    }
  }
  return validPasswordCount;
}

TEST_CASE("Day02", "[02]") {
  REQUIRE(answer(true) == 422);
  REQUIRE(answer(false) == 451);
}
