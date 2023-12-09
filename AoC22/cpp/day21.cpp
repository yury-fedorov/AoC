#include "absl/container/flat_hash_map.h"
#include "common.h"
#include "re2/re2.h"

namespace day21 {

using Long = int64_t;
using Int = double;
using Var = std::pair<Long, std::string>;
using Map = absl::flat_hash_map<std::string, Var>;

constexpr std::string_view kRoot = "root";
constexpr std::string_view kHuman = "humn";

template <class T>
[[nodiscard]] T EvalT(T a, std::string_view op, T b) noexcept {
  switch (op[0]) {
    case '+':
      return a + b;
    case '-':
      return a - b;
    case '*':
      return a * b;
    case '/': {
      return a / b;
    }
  }
  return std::numeric_limits<T>::quiet_NaN();
}

template <class T>
[[nodiscard]] T Eval(Map& map, std::string_view node) {
  const Var& var = map.at(node);
  const auto [raw_val, formula] = var;
  if (formula.empty()) return raw_val;

  static re2::RE2 re("([a-z]{4}) (.) ([a-z]{4})");
  re2::StringPiece input(formula);
  std::string a, op, b;
  if (re2::RE2::FullMatch(input, re, &a, &op, &b)) {
    const auto av = Eval<T>(map, a);
    const auto bv = Eval<T>(map, b);
    return EvalT(av, op, bv);
  }

  if (Long value{-1}; absl::SimpleAtoi(formula, &value)) {
    map.at(node) = Var{value, ""};
    return value;
  }
  return std::numeric_limits<T>::quiet_NaN();
}

[[nodiscard]] Map LoadMap(std::string_view file) noexcept {
  const auto data = ReadData(file);
  // root: pppw + sjmn
  // dbpl: 5
  re2::RE2 re("([a-z]{4}): (.+)");
  Map map;
  for (const auto& line : data) {
    if (line.empty()) continue;
    re2::StringPiece input(line);
    std::string node, formula;
    if (re2::RE2::FullMatch(input, re, &node, &formula)) {
      map.insert({node, Var{0, formula}});
    } else {
      EXPECT_TRUE(false) << line;
    }
  }
  return map;
}

[[nodiscard]] Long Answer1(Map map) noexcept {
  return Eval<Long>(map, kRoot);
}

double PureEval(Map map, std::string_view root, Long humn) {
    map[kHuman] = Var{ humn, "" };
    return Eval<double>(map, root);
}

[[nodiscard]] bool IsSensible(const Map& map, std::string_view root) noexcept {
  const auto zero = PureEval(map, root, 0);
  const auto y1 = PureEval(map, root, 100);
  return y1 != zero;
}

[[nodiscard]] Int Answer2(const Map& map, std::string_view root_var, Int target) noexcept {
    // f(x) = a + b*x
    const auto f = [&map, &root_var](Int x) { return PureEval(map, root_var, x); };
    const Int x0 = 0;
    const Int y0 = f(x0);
    const Int x1 = 1'000'000; // sensitivity of result
    const Int y1 = f(x1);
    const Int b = (y1 - y0) / (x1 - x0);
    const Int a = y0;
    return round( (target - a) / b );
}

[[nodiscard]] Int Answer2(const Map& map) noexcept {
  const std::string old_root = map.at(kRoot).second;  // contains +
  const std::vector<std::string> parts = absl::StrSplit(old_root, " + ");
  const std::string_view root_a = parts.front();
  const std::string_view root_b = parts.back();
  const bool is_a_sens = IsSensible(map, root_a);
  const auto& root_const = is_a_sens ? root_b : root_a;
  const auto& root_var = is_a_sens ? root_a : root_b;
  const Int target = PureEval(map, root_const, std::numeric_limits<Long>::quiet_NaN());
  return Answer2(map, root_var, target);
}

void Solution(std::string_view file, Int answer1, Int answer2) noexcept {
  const auto map = day21::LoadMap(file);
  EXPECT_EQ(Answer1(map), answer1);
  EXPECT_EQ(Answer2(map), answer2);
}

}  // namespace day21

TEST(AoC22, Day21) {
  day21::Solution("21-sample", 152, 301);
  day21::Solution("21", 256997859093114LL, 3952288690726LL);
}
