#include "absl/container/flat_hash_map.h"
#include "common.h"
#include <re2/re2.h>

namespace day21 {

using Long = long long;
// using Int = __int128;
using Int = Long;
using Var = std::pair<Int, std::string>;
// using Map = absl::flat_hash_map<std::string, Var>;
using Map = std::map<std::string, Var>;

[[nodiscard]] Int Eval(Int a, std::string_view op, Int b) noexcept {
  switch (op[0]) {
  case '+':
    return a + b;
  case '-':
    return a - b;
  case '*': {
    const auto ab = a * b;
    if (ab < a || ab < b || ab < 0)
      EXPECT_TRUE(false) << a << "*" << b;
    return a * b;
  }
  case '/': {
    const auto rem = a % b;
    if (rem != 0)
      EXPECT_TRUE(false) << a << "/" << b;
    return a / b;
  }
  }
  EXPECT_TRUE(false) << "Unexpected op: " << op;
  return 0;
}

[[nodiscard]] Int Eval(const std::string & node, Map &map) noexcept {
  const Var & var = map.at(node);
  const auto [raw_val, formula] = var;
  if (formula.empty())
    return raw_val;

  static re2::RE2 re("([a-z]{4}) ([+-/*]) ([a-z]{4})");
  re2::StringPiece input(formula);
  std::string a, op, b;
  if (re2::RE2::FullMatch(input, re, &a, &op, &b)) {
    const Int av = Eval(a, map);
    const Int bv = Eval(b, map);
    const Int value = Eval(av, op, bv);
    if ( value < 0 )
      EXPECT_TRUE(false) << formula << " " << av << op << bv << " = " << value;
    map.at(node) = Var{value, ""}; // calculate only once
    return value;
  }

  if (Long value {-1}; absl::SimpleAtoi(formula, &value)) {
    if ( value < 0 )
      EXPECT_TRUE(false) << formula << " " << value;
    map.at(node) = Var{value, ""};
    return value;
  }
  EXPECT_TRUE(false) << "(" << node << " -> " << formula << ")";
  return 0;
}

[[nodiscard]] int Answer1(std::string_view file) noexcept {
  const auto data = ReadData(file);
  // root: pppw + sjmn
  // dbpl: 5
  re2::RE2 re("([a-z]{4}): (.+)");
  Map map;
  for (const auto &line : data) {
    if (line.empty())
      continue;
    re2::StringPiece input(line);
    std::string node, formula;
    if (re2::RE2::FullMatch(input, re, &node, &formula)) {
      map.insert({node, Var{0, formula}});
    } else {
      EXPECT_TRUE(false) << line;
    }
  }
  return Eval("root", map);
}

} // namespace day21

TEST(AoC22, Day21) {
  EXPECT_EQ(day21::Answer1("21-sample"), 152);
  EXPECT_EQ(day21::Answer1("21"), day21::Long{0} ); // - -98997638 -98997638
}
