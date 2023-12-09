#include "absl/container/flat_hash_map.h"
#include "common.h"
#include "re2/re2.h"

namespace day21 {

using Long = long long;
#ifdef __SIZEOF_INT128__
using Longest = __int128;
#else
// using Longest = absl::int128;
using Longest = Long;
#endif

// using Int = int64_t;
// using Int = Longest;
using Int = int64_t;  // 2530*1419951562 = 1884792404
using Var = std::pair<Int, std::string>;
using Map = absl::flat_hash_map<std::string, Var>;
// using Map = std::map<std::string, Var>;

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
      const auto r = a % b;
      if (r != 0) return std::optional<T>();  // not suitable
      return a / b;
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
  if (!result_option.has_value()) return std::optional<Int>();
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

[[nodiscard]] std::optional<Int> Eval(const std::string& node,
                                      Map& map) noexcept {
  const Var& var = map.at(node);
  const auto [raw_val, formula] = var;
  if (formula.empty()) return raw_val;

  static re2::RE2 re("([a-z]{4}) (.) ([a-z]{4})");
  re2::StringPiece input(formula);
  std::string a, op, b;
  if (re2::RE2::FullMatch(input, re, &a, &op, &b)) {
    const auto av = Eval(a, map);
    if (!av.has_value()) return std::optional<Int>();
    const auto bv = Eval(b, map);
    if (!bv.has_value()) return std::optional<Int>();
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
    if (value < 0) EXPECT_TRUE(false) << formula << " " << value;
    map.at(node) = Var{value, ""};
    return value;
  }
  EXPECT_TRUE(false) << "(" << node << " -> " << formula << ")";
  return std::optional<Int>();
}

[[nodiscard]] std::optional<Int> PureEval(std::string_view node,
                                          Map map) noexcept {
  return Eval(std::string(node), map);
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

[[nodiscard]] Int Answer1(Map map) noexcept {
  return Eval("root", map).value();
}

constexpr std::string_view kHuman = "humn";

[[nodiscard]] bool IsSensible(const Map& map, std::string_view root) noexcept {
  std::array shifts = {1, 5, 20, 250, 1000};
  const auto original_root =
      PureEval(root, map).value();  // this is always expected to be ok
  Map map1 = map;
  const auto zero = PureEval(kHuman, map).value();
  for (const int shift : shifts) {
    map1[kHuman] = Var{zero + shift, ""};
    const auto shifted_root_option = PureEval(root, map);
    EXPECT_TRUE(shifted_root_option.has_value());
    if (shifted_root_option.value() != original_root) return true;
  }
  return false;
}

[[nodiscard]] Int Answer2(const Map& map, std::string_view root_var, Int target,
                          Int min, Int max) noexcept {
  EXPECT_TRUE(min < max) << "Unexpected input " << min << " " << max;

  Map map1 = map;
  const Int min_max_diff{max - min};
  if (abs(min_max_diff) < 100000LL) {
    // simplified
    for (Int result = min; result <= max; result++) {
      map1[kHuman] = Var{result, ""};
      const auto opt = PureEval(root_var, map1);
      if (!opt.has_value()) continue;
      if (target == opt.value()) return result;
    }
    EXPECT_TRUE(false) << "Failed to find a solution: " << min << " " << max
                       << " diff " << min_max_diff;
    return -1;
  }

  Int mid = min < 0 && max > 0 ? 0 : (min + max) / 2;
  EXPECT_TRUE(min < mid && mid < max) << min << " " << mid << " " << max;
  Int diff_min, diff_mid, diff_max;
  while (true) {
    map1[kHuman] = Var{min, ""};
    const auto opt_min = PureEval(root_var, map1);
    if (opt_min.has_value()) {
      diff_min = abs(target - opt_min.value());
      break;
    }
    min++;
  }
  while (true) {
    map1[kHuman] = Var{max, ""};
    const auto opt_max = PureEval(root_var, map1);
    if (opt_max.has_value()) {
      diff_max = abs(target - opt_max.value());
      break;
    }
    max--;
  }
  while (true) {
    map1[kHuman] = Var{mid, ""};
    const auto opt_mid = PureEval(root_var, map1);
    if (opt_mid.has_value()) {
      diff_mid = abs(target - opt_mid.value());
      break;
    }
    mid--;
  }
  EXPECT_TRUE(min < mid && mid < max)
      << "Post calibration " << min << " " << mid << " " << max;
  if (diff_min < diff_max) {
    max = mid;
  } else {
    min = mid;
  }
  return Answer2(map, root_var, target, min, max);
}

[[nodiscard]] Int Answer2(const Map& map) noexcept {
  const std::string old_root = map.at("root").second;  // contains +
  const std::vector<std::string> parts = absl::StrSplit(old_root, " + ");
  std::string root_a = parts.front();
  std::string root_b = parts.back();
  const bool is_a_sens = IsSensible(map, root_a);
  const bool is_b_sens = IsSensible(map, root_b);
  EXPECT_FALSE(is_a_sens && is_b_sens);  // we assume that only one is sensible
  const auto& root_const = is_a_sens ? root_b : root_a;
  const auto& root_var = is_a_sens ? root_a : root_b;
  const Int target = PureEval(root_const, map).value();
  constexpr Int min{0};
  constexpr Int max{LLONG_MAX / 10'000};
  return Answer2(map, root_var, target, min, max);
}

void Solution(std::string_view file, Int answer1, Int answer2) noexcept {
  const auto map = day21::LoadMap(file);
  EXPECT_EQ(Answer1(map), answer1);
  // TODO - EXPECT_EQ(Answer2(map), answer2);
}

}  // namespace day21

TEST(AoC22, Day21) {
  day21::Solution("21-sample", 152, 301);
  day21::Solution("21", 256997859093114LL, -1);  // 2326
}
