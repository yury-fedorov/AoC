#include <algorithm>
#include <assert.h>
#include <catch2/catch.hpp>
#include <climits>
#include <fstream>
#include <iostream>
#include <map>
#include <numeric>
#include <regex>
#include <set>
#include <sstream>
#include <vector>

using namespace std;

TEST_CASE("Day09", "[09]") {
  ifstream f("09/input.txt");

  long maxDistance = 0;
  long minDistance = LONG_MAX;

  map<pair<string, string>, int> routes;
  set<string> sites;
  regex re("(.+) to (.+) = (\\d+)");

  string line;
  while (getline(f, line)) {
    smatch what;
    if (regex_match(line, what, re)) {
      const auto &a = what[1];
      const auto &b = what[2];
      const int d = stoi(what[3]);
      routes.emplace(make_pair(a, b), d);
      sites.insert(a);
      sites.insert(b);
    } else {
      cerr << "Unexpected line: " << line << endl;
      FAIL();
    }
  }
  const int siteCount(sites.size());
  vector<string> siteList(sites.begin(), sites.end());
  vector<int> route(siteCount);
  iota(route.begin(), route.end(), 0);

  const auto noRoute = routes.end();

  do {
    long distance = 0;
    auto a = siteList[route[0]];
    for (auto i = 1; i < siteCount; i++) {
      const string &b = siteList[route[i]];
      assert(a != b);
      const auto ab = routes.find(make_pair(a, b));
      if (ab != noRoute) {
        distance += ab->second;
      } else {
        const auto ba = routes.find(make_pair(b, a));
        if (ba != noRoute) {
          distance += ba->second;
        } else {
          cerr << "No route found between " << a << " and " << b << endl;
          FAIL();
        }
      }
      a = b;
    }
    minDistance = min(minDistance, distance);
    maxDistance = max(maxDistance, distance);
  } while (std::next_permutation(route.begin(), route.end()));
  REQUIRE(207 == minDistance);
  REQUIRE(804 == maxDistance);
}