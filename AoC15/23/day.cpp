#include <assert.h>
#include <catch2/catch.hpp>
#include <fstream>
#include <iostream>
#include <map>
#include <regex>
#include <vector>

using namespace std;

typedef pair<string, string> Instruction;
typedef vector<Instruction> Program;
typedef map<char, int> Registers;

inline char argsToRegister(const string &args) {
  assert(args.length() == 1);
  return args[0];
}

pair<char, int> argsToRegOffset(const string &args) {
  static const regex re2arg("^(\\w), (.+)$");
  smatch what;
  if (regex_match(args, what, re2arg)) {
    return {what[1].str()[0], stoi(what[2])};
  }
  cerr << "Unexpected line: " << args << endl;
  assert(false);
}

auto day23(const bool isFirstAnswer) {
  Program program;
  ifstream f("23/input");
  const regex re("^(\\w{3}) (.+)$");

  string line;
  while (getline(f, line)) {
    smatch what;
    if (regex_match(line, what, re)) {
      program.emplace_back(what[1], what[2]);
    } else {
      cerr << "Unexpected line: " << line << endl;
      FAIL();
    }
  }

  const int n = program.size();
  Registers registers;
  if (!isFirstAnswer)
    registers['a'] = 1;
  for (int curOp = 0; curOp >= 0 && curOp < n;) {
    const auto [cmd, args] = program[curOp];
    if (cmd == "hlf") {
      registers[argsToRegister(args)] /= 2;
      curOp++;
    } else if (cmd == "tpl") {
      registers[argsToRegister(args)] *= 3;
      curOp++;
    } else if (cmd == "inc") {
      registers[argsToRegister(args)] += 1;
      curOp++;
    } else if (cmd == "jmp") {
      curOp += stoi(args);
    } else if (cmd == "jie") {
      const auto [r, offset] = argsToRegOffset(args);
      curOp += (registers[r] % 2 == 0) ? offset : 1;
    } else if (cmd == "jio") {
      const auto [r, offset] = argsToRegOffset(args);
      curOp += (registers[r] == 1) ? offset : 1;
    } else
      assert(false);
  }
  return registers['b'];
}

TEST_CASE("Day23", "[23]") {
  REQUIRE(184 == day23(true));
  REQUIRE(231 == day23(false));
}