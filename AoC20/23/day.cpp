#include <algorithm>
#include <assert.h>
#include <catch2/catch.hpp>
#include <iostream>
#include <list>
#include <map>

using namespace std;

typedef int Cup;
typedef list<Cup> Cups;
typedef list<Cup>::iterator CupIter;
typedef map<Cup, CupIter> CupIterMap;

string answer23(const Cups &cups) {
  string result;
  auto i = find(cups.cbegin(), cups.cend(), 1);
  for (auto j = next(i); j != i; j++) {
    if (j == cups.cend())
      j = cups.cbegin();
    result += to_string(*j);
  }
  return result;
}

void play(Cups &cups, CupIterMap &map, int n) {
  const int maxElement = *max_element(cups.cbegin(), cups.cend());
  auto cur = cups.begin();
  for (int i = 1; i <= n; i++) {
    Cups pickUp;
    for (int j = 0; j < 3; j++) {
      auto cur1 = next(cur);
      if (cur1 == cups.end())
        cur1 = cups.begin();
      pickUp.push_back(*cur1);
      cups.erase(cur1);
    }
    for (int prev = *cur - 1; true; prev--) {
      if (prev < 1) {
        for (prev = maxElement;
             find(pickUp.begin(), pickUp.end(), prev) != pickUp.end(); prev--) {
        }
      } else if (find(pickUp.begin(), pickUp.end(), prev) != pickUp.end())
        continue;
      auto i = map.at(prev);
      if (i != cups.end()) {
        auto j = next(i);
        auto ri = cups.insert(j, pickUp.begin(), pickUp.end());
        for (const auto pue : pickUp) {
          map[pue] = ri++;
        }
        break;
      }
    }
    cur++;
    if (cur == cups.end())
      cur = cups.begin();
  }
}

string day23(const bool isFirstAnswer) {
  const string input = "463528179";
  // const string input = "32415"; // test
  // const string input = "389125467"; // test1
  // const string input = "123456789";

  Cups cups;
  CupIterMap map;
  for (const char ch : input) {
    const int e = ch - '0';
    map.emplace(e, cups.insert(cups.end(), e));
  }

  if (isFirstAnswer) {
    play(cups, map, 100);
    return answer23(cups);
  } else {
    int i = *max_element(cups.cbegin(), cups.cend()) + 1;
    const int n = 1'000'000;
    for (; i <= n; i++) {
      map.emplace(i, cups.insert(cups.end(), i));
    }
    assert(cups.size() == n);
    assert(map.size() == n);
    play(cups, map, 10'000'000);
    auto j = find(cups.cbegin(), cups.cend(), 1);
    if (++j == cups.cend())
      j = cups.cbegin();
    const unsigned long long a = *j;
    if (++j == cups.cend())
      j = cups.cbegin();
    const unsigned long long b = *j;
    const unsigned long long answer2 = a * b;
    return to_string(answer2);
  }
}

TEST_CASE("Day23-SLOW", "[23]") {
  REQUIRE("52937846" == day23(true));
  return;
  REQUIRE("8456532414" == day23(false)); // takes 63 seconds
}