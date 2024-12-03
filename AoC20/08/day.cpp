#include <algorithm>
#include <assert.h>
#include <catch2/catch.hpp>
#include <fstream>
#include <iostream>
#include <optional>
#include <regex>
#include <set>
#include <vector>

using namespace std;

const vector<string> operations{"nop", "acc", "jmp"};
enum Operation { NOP, ACC, JMP };
typedef pair<Operation, int> Instruction;

pair<bool, int> run(const vector<Instruction> &code) {
  int accumulator{0};
  size_t next{0};
  set<int> executed;

  while (executed.find(next) == executed.end()) {
    executed.insert(next);
    const Instruction &i = code[next];
    switch (i.first) {
    case NOP:
      next++;
      break;
    case ACC:
      accumulator += i.second;
      next++;
      break;
    case JMP:
      next += i.second;
      break;
    default:
      cerr << next << ": " << i.first << " " << i.second << endl;
      FAIL();
    }
    if (next == code.size())
      return make_pair(true, accumulator); // proper termination
  }
  return make_pair(false, accumulator);
}

TEST_CASE("Day08", "[08]") {
  ifstream f("08/input.txt");
  regex re("^(\\w{3}) ([-+]\\d+)$");
  smatch what;
  string line;
  vector<Instruction> code;
  while (getline(f, line)) {
    if (regex_match(line, what, re)) {
      const string op = what[1];
      const int arg = stoi(what[2]);
      const auto i = find(operations.cbegin(), operations.cend(), op);
      assert(i != operations.end());
      code.emplace_back((Operation)(int)(i - operations.cbegin()), arg);
    } else {
      cerr << "Unexpected line: " << line << endl;
      FAIL();
    }
  }

  const auto r = run(code);
  REQUIRE(1563 == r.second);

  for (auto &i : code) {
    const Operation originalOp = i.first;
    typedef optional<Operation> OptOp;
    const auto newOp = (originalOp == NOP)   ? OptOp(JMP)
                       : (originalOp == JMP) ? OptOp(NOP)
                                             : OptOp();
    if (newOp.has_value()) {
      i.first = newOp.value();
      const auto r2 = run(code);
      if (r2.first) {
        REQUIRE(767 == r2.second);
        return;
      }
      i.first = originalOp;
    }
  }
  FAIL();
}
