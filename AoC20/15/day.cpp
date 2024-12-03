#include <assert.h>
#include <catch2/catch.hpp>
#include <fstream>
#include <iostream>
#include <map>
#include <regex>
#include <vector>

using namespace std;

auto day15(const bool isFirstAnswer) {
  vector<int> s; // sequence
  ifstream f("15/input.txt");
  string line;
  f >> line;
  regex re("(\\d+)");
  smatch res;
  string::const_iterator searchStart(line.cbegin());
  while (regex_search(searchStart, line.cend(), res, re)) {
    const auto n = stoi(res[1]);
    s.push_back(n);
    searchStart = res.suffix().first;
  }

  const int index = isFirstAnswer ? 2020 : 30'000'000;
  map<int, int> lastMap;
  for (size_t i = 0; i < s.size() - 1; i++) {
    lastMap[s[i]] = i;
  }
  int last = *s.rbegin();
  for (int i = s.size(); i < index; i++) {
    const auto j = lastMap.find(last);
    const int next = (j == lastMap.end()) ? 0 : (i - j->second - 1);
    lastMap[last] = i - 1; // previous element
    if (i >= index)
      break;
    last = next;
  }
  return last;
}

TEST_CASE("Day15", "[15]") {
  REQUIRE(1015 == day15(true));
  const auto runSlow = false;
  if (!runSlow)
    return;
  REQUIRE(201 == day15(false)); // slow 45 seconds
}
