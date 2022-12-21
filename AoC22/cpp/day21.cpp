#include "absl/container/flat_hash_map.h"
#include "common.h"
#include <re2/re2.h>

namespace day21 {

using Long = long long;
// using Longest = absl::int128;
using Longest = __int128;
// using Int = int64_t;
// using Int = Longest;
using Int = int64_t; // 2530*1419951562 = 1884792404
using Var = std::pair<Int, std::string>;
// using Map = absl::flat_hash_map<std::string, Var>;
using Map = std::map<std::string, Var>;

template <class T>
[[nodiscard]] std::optional<T> EvalT(T a, std::string_view op, T b) noexcept {
  switch (op[0]) {
  case '+':
    return a + b;
  case '-':
    return a - b;
  case '*':
    return a * b;
  case '/': {
    {
      const auto r = a % b;
      if (r != 0)
        return std::optional<T>(); // not siutable
      return a / b;
    }
  case '=':
    return a == b ? 1 : 0;
  }
  }
  EXPECT_TRUE(false) << "Unexpected op: " << op;
  return std::optional<T>();
}

[[nodiscard]] std::string ToStr(Int v) noexcept {
  return absl::StrCat(static_cast<int64_t>(v));
}

[[nodiscard]] std::optional<Int> Eval(Int a, std::string_view op,
                                      Int b) noexcept {
  const auto result_option = EvalT<Longest>(a, op, b);
  if (!result_option.has_value())
    return std::optional<Int>();
  const auto result = result_option.value();
  if (result < 0) {
    // EXPECT_TRUE(false) << "Negative number: " << ToStr(a)
    //             << op << ToStr(b) << " = "
    //             << ToStr(result);
    return std::optional<Int>();
  }
  // const auto alt = EvalT<long double>(a, op, b);
  // const auto d = abs( alt.value_or(0) - result );
  if (std::in_range<Int>(result) /* && d < 0.0001 */)
    return static_cast<Int>(result);
  EXPECT_TRUE(false) << "Cast cannot be done: " << ToStr(a) << op << ToStr(b)
                     << " = " << ToStr(result);
  return std::optional<Int>();
}

[[nodiscard]] std::optional<Int> Eval(const std::string &node,
                                      Map &map) noexcept {
  const Var &var = map.at(node);
  const auto [raw_val, formula] = var;
  if (formula.empty())
    return raw_val;

  static re2::RE2 re("([a-z]{4}) ([+-/*=]) ([a-z]{4})");
  re2::StringPiece input(formula);
  std::string a, op, b;
  if (re2::RE2::FullMatch(input, re, &a, &op, &b)) {
    const auto av = Eval(a, map);
    if (!av.has_value())
      return std::optional<Int>();
    const auto bv = Eval(b, map);
    if (!bv.has_value())
      return std::optional<Int>();
    const auto value_option = Eval(av.value(), op, bv.value());
    if (!value_option.has_value()) {
      // EXPECT_TRUE(false) << formula << " " << ToStr(av) << op << ToStr(bv);
      return std::optional<Int>();
    }
    const auto value = value_option.value();
    // map.at(node) = Var{value, ""};  // calculate only once
    return value;
  }

  if (Long value{-1}; absl::SimpleAtoi(formula, &value)) {
    if (value < 0)
      EXPECT_TRUE(false) << formula << " " << value;
    map.at(node) = Var{value, ""};
    return value;
  }
  EXPECT_TRUE(false) << "(" << node << " -> " << formula << ")";
  return std::optional<Int>();
}

[[nodiscard]] Int Answer1(std::string_view file) noexcept {
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
  return Eval("root", map).value();
}

/*
constexpr std::string_view k_human = "humn";

[[nodiscard]] std::optional<Var> Eval2(const std::string &node, Map &map)
noexcept { if ( node == k_human ) return Var { 0, k_human }; // we do formula

  const Var &var = map.at(node);
  const auto [raw_val, formula] = var;
  if (formula.empty()) return raw_val;

  static re2::RE2 re("(.*) ([+-/*=]) (.*})");
  re2::StringPiece input(formula);
  std::string a, op, b;
  if (re2::RE2::FullMatch(input, re, &a, &op, &b)) {
    const auto av = Eval(a, map);
    if ( !av.has_value() ) return std::optional<Var>();
    const auto bv = Eval(b, map);
    if ( !bv.has_value() ) return std::optional<Var>();
    const auto value_option = Eval2(av.value(), op, bv.value());
    if ( !value_option.has_value() ) {
      // EXPECT_TRUE(false) << formula << " " << ToStr(av) << op << ToStr(bv);
      return std::optional<Var>();
    }
    const auto value = value_option.value();
    // map.at(node) = Var{value, ""};  // calculate only once
    return value;
  }

  if (Long value{-1}; absl::SimpleAtoi(formula, &value)) {
    if (value < 0) EXPECT_TRUE(false) << formula << " " << value;
    map.at(node) = Var{value, ""};
    return value;
  }
  EXPECT_TRUE(false) << "(" << node << " -> " << formula << ")";
  return std::optional<Var>();
}

[[nodiscard]] Int Answer2(std::string_view file, Int start = 1) noexcept {
  const auto data = ReadData(file);
  // root: pppw + sjmn
  // dbpl: 5
  re2::RE2 re("([a-z]{4}): (.+)");
  Map map;
  for (const auto &line : data) {
    if (line.empty()) continue;
    re2::StringPiece input(line);
    std::string node, formula;
    if (re2::RE2::FullMatch(input, re, &node, &formula)) {
      map.insert({node, Var{0, formula}});
    } else {
      EXPECT_TRUE(false) << line;
    }
  }
  const std::string old_root = map.at( "root").second;
  const std::string new_root = absl::StrReplaceAll( old_root, { { "+", "=" } }
); map["root"] = Var { 0, new_root }; const Map original { map }; for ( Int i =
start; i < std::numeric_limits<Int>::max(); i++ ) { Map m { original };
        m["humn"] = Var{i,""};
        if ( Eval("root", m).value_or(0) == 1 ) return i;
  }
  return -1;
}
*/

} // namespace day21

TEST(AoC22, Day21) {
  EXPECT_EQ(day21::Answer1("21-sample"), 152);
  EXPECT_EQ(day21::Answer1("21"), 256997859093114LL);

  // EXPECT_EQ(day21::Answer2("21-sample"), 301);
  // EXPECT_EQ(day21::Answer2("21", 2326 +1), 0); // 2326
}
