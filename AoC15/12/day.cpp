#include <catch2/catch.hpp>
#include <fstream>
#include <iostream>
#include <regex>
#include <sstream>
#include <string_view>

using namespace std;

TEST_CASE("Day12", "[12]") {
  ifstream f("12/input.txt");

  long long sum = 0;

  string line;
  while (getline(f, line)) {
    regex exp("(-?\\d+)");
    smatch res;
    string::const_iterator searchStart(line.cbegin());
    while (regex_search(searchStart, line.cend(), res, exp)) {
      sum += stoi(res[0]);
      searchStart = res.suffix().first;
    }
  }

  REQUIRE(156366 == sum); // answer 1
}
