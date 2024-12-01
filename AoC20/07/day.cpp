#include <algorithm>
#include <catch2/catch.hpp>
#include <fstream>
#include <iostream>
#include <map>
#include <regex>
#include <vector>

using namespace std;

typedef map<string, vector<pair<string, int>>> Rules;

bool canContain(const Rules &rules, const string &out, const string &in) {
  for (const auto &p : rules.at(out)) {
    if ((p.first == in) || canContain(rules, p.first, in))
      return true;
  }
  return false;
}

int countIn(const Rules &rules, const string &out) {
  int result = 0;
  for (const auto &p : rules.at(out)) {
    result += p.second * (1 + countIn(rules, p.first));
  }
  return result;
}

TEST_CASE("Day07", "[07]") {
  ifstream f("07/input.txt");
  // ifstream f("test.txt");

  string line;

  // drab tan bags contain 4 clear gold bags.
  // vibrant lime bags contain 3 faded gold bags, 3 plaid aqua bags, 2 clear
  // black bags.
  const regex reLine("^([\\w ]+) bags contain ([^.]+).$");
  const regex reList("([\\d]+) ([\\w ]+) bag");
  Rules rules;

  while (getline(f, line)) {
    smatch what;
    if (regex_match(line, what, reLine)) {
      const string container = what[1];
      const string all = what[2];

      vector<pair<string, int>> list;

      string::const_iterator searchStart(all.cbegin());
      while (regex_search(searchStart, all.cend(), what, reList)) {
        list.emplace_back(what[2], stoi(what[1]));
        searchStart = what.suffix().first;
      }
      rules.insert(make_pair(container, list));
    } else {
      cerr << "Unexpected line: " << line << endl;
    }
  }

  auto count = 0;
  const string color = "shiny gold";
  for (auto &p : rules) {
    count += canContain(rules, p.first, color);
  }
  REQUIRE(370 == count);
  REQUIRE(29547 == countIn(rules, color));
}