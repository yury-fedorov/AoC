#include <assert.h>
#include <catch2/catch.hpp>
#include <fstream>
#include <iostream>
#include <regex>
#include <vector>

using namespace std;

typedef unsigned long long Int;

Int evaluate(const string &e, const bool isFirstAnswer) {
  stack<string> s;
  string level;
  for (const char ch : e) {
    if (ch == '(') {
      s.push(level);
      level = "";
    } else if (ch == ')') {
      const string level1 = s.top();
      s.pop();
      const auto value = evaluate(level, isFirstAnswer);
      level = level1 + to_string(value);
    } else {
      level += ch;
    }
  }
  assert(s.empty());
  // qui level is flat: no parantesis
  static const regex re("([^ ]+)");
  smatch res;
  string::const_iterator searchStart(level.cbegin());
  vector<Int> prod;
  while (regex_search(searchStart, level.cend(), res, re)) {
    const string term = res[1];
    const bool isOperator = term == "+" || term == "*";
    if (isOperator || s.size() < 2) {
      s.push(term); // +1
    } else {
      // neither operator, nor first argument, we can calculate
      const bool isSum = s.top() == "+";
      if (isFirstAnswer || isSum) {
        s.pop();
        const auto arg1 = stoull(s.top());
        s.pop();
        const auto arg2 = stoull(term);
        const auto r = isSum ? (arg1 + arg2) : (arg1 * arg2);
        s.push(to_string(r));
      } else {
        // it is product
        s.pop();                         // throug away product operator
        prod.push_back(stoull(s.top())); // product arg
        s.pop();
        s.push(term); // now the time for eventual further sum
      }
    }
    searchStart = res.suffix().first;
  }
  assert(s.size() == 1);
  Int num = stoull(s.top());
  for (const auto a : prod) {
    num *= a;
  }
  return num;
}

auto day18(const bool isFirstAnswer) {
  Int sum{0};
  ifstream f("18/input");
  string line;
  while (getline(f, line)) {
    const auto r = evaluate(line, isFirstAnswer);
    sum += r;
  }
  return sum;
}

TEST_CASE("Day18", "[18]") {
  REQUIRE(3885386961962 == day18(true));
  REQUIRE(112899558798666 == day18(false));
}