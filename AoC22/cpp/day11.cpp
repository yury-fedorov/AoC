#include "absl/status/statusor.h"
#include "absl/strings/numbers.h"
#include "common.h"
#include <re2/re2.h>

namespace day11 {
using Longest = __int128;
using Long = long long;
using BigInt = std::pair<Longest,Long>; // x, y in formula modulo * x + y
using Operation = std::function<BigInt(BigInt)>;

constexpr bool IS_TEST = false;
constexpr Long MODULO = IS_TEST ? 96577 : 9699690; // TODO proper modulo

BigInt Compress( BigInt a, Long modulo ) {
  const auto p = std::div( a.second, modulo );
  return { a.first + p.quot, p.rem };
}

BigInt Add( BigInt a, BigInt b ) {
  return { a.first + b.first, a.second + b.second }; // not compressed
}

BigInt Mul( BigInt a, BigInt b, Long modulo ) {
  const auto [an,a_] = a;
  const auto [bn,b_] = b;
  const BigInt ab = { ( an * bn * modulo ) + ( an * b_ ) + ( bn * a_ ), a_ * b_ };
  return Compress( ab, modulo );
}

// very basic just for part 1
BigInt Div( BigInt a, int base ) {
  return { a.first, a.second / base };
}

bool IsDiv( BigInt a, int dividedBy ) {
  // modulo must be dividable
  return a.second % dividedBy == 0;
}

BigInt ToInt( int value ) { return { 0, value }; }
Longest FromInt( BigInt i, Long modulo ) { return modulo * i.first + i.second; }

struct Monkey {
  std::vector<BigInt> items;
  Operation operation;
  int divisibleBy;
  int monkeyTrue;
  int monkeyFalse;
  Monkey(std::vector<BigInt> items_, Operation operation_, int divisibleBy_,
         int monkeyTrue_, int monkeyFalse_)
      : items(items_), operation(operation_), divisibleBy(divisibleBy_),
        monkeyTrue(monkeyTrue_), monkeyFalse(monkeyFalse_) {}
  Monkey(const Monkey &other) = default;
};
// constexpr
Operation OpAdd(int arg) {
  return [arg](BigInt old) -> BigInt { return Add( old, ToInt( arg) ); };
};
Operation OpMul(int arg) {
  return [arg](BigInt old) -> BigInt { return Mul( old, ToInt( arg ), MODULO ); };
};

absl::StatusOr<Operation> OpCreate(std::string_view operation) {
  // new = old * 17, new = old * old
  std::string sign, arg2;

  re2::StringPiece input(operation);
  re2::RE2 re("new = old ([+*]) (.*)");

  if (re2::RE2::FullMatch(input, re, &sign, &arg2)) {
    if (int argument{0}; absl::SimpleAtoi(arg2, &argument)) {
      // the second argument is number
      if (sign == "+")
        return OpAdd(argument);
      if (sign == "*")
        return OpMul(argument);
      return absl::InvalidArgumentError(
          absl::StrCat("Unexpected sign: ", operation));
    }
    if (operation != "new = old * old")
      return absl::InvalidArgumentError("unknown case of not digital argument");
    return [](BigInt old) -> BigInt { return Mul( old, old, MODULO); };
  }
  return absl::InvalidArgumentError(
      absl::StrCat("Unexpected pattern: ", operation));
}

// TODO - ranges / views?
std::vector<BigInt> ToInt(const std::vector<std::string> &v) {
  std::vector<BigInt> result;
  for (const auto &l : v) {
    if (int value{0}; absl::SimpleAtoi(l, &value)) {
      result.push_back( ToInt( value ) );
    }
  }
  return result;
}

absl::StatusOr<int> ReadTailNumber(std::string_view line) {
  int value{0};
  const int start_pos = line.find_last_of(' ');
  const std::string number(line.data() + start_pos, line.end());
  if (absl::SimpleAtoi(number, &value))
    return value;
  return absl::InvalidArgumentError(absl::StrCat("unexpected input: ", line));
}

} // namespace day11

TEST(AoC22, Day11) {

  EXPECT_EQ( day11::FromInt( day11::OpCreate("new = old * 17").value()(day11::ToInt(1)), day11::MODULO ), 17);
  // EXPECT_EQ(day11::OpCreate("new = old + 17").value()(1), 18);
  // EXPECT_EQ(day11::OpCreate("new = old * old").value()(2), 4);
  const bool is_part_1 = false;
  const auto n = is_part_1 ? 20 : 10000;

  const auto data = ReadData( day11::IS_TEST ? "11-sample" : "11");
  std::vector<day11::Monkey> monkeys;

  for (int i = 0; i < data.size(); i++) {
    const auto &line = data[i];
    if (!line.starts_with("Monkey"))
      continue;
    // found a monkey description
    const std::vector<std::string> parts_items =
        absl::StrSplit(data[i + 1], ": ");
    const std::vector<day11::BigInt> items =
        day11::ToInt(absl::StrSplit(parts_items[1], ", "));

    const std::vector<std::string> parts_operation =
        absl::StrSplit(data[i + 2], "Operation: ");
    const auto op_status = day11::OpCreate(parts_operation[1]);

    const auto divisibleBy = day11::ReadTailNumber(data[i + 3]);
    const auto monkeyTrue = day11::ReadTailNumber(data[i + 4]);
    const auto monkeyFalse = day11::ReadTailNumber(data[i + 5]);

    monkeys.emplace_back(day11::Monkey(items, op_status.value(),
                                       divisibleBy.value(), monkeyTrue.value(),
                                       monkeyFalse.value()));
  }
  // constexpr int n_monkeys = 8;
  // EXPECT_EQ(monkeys.size(), n_monkeys);
  std::vector<day11::Long> counts(monkeys.size(), 0); // all values set to 0

  day11::Long modulo = {1};
  for ( auto &m : monkeys ) {
    modulo *= m.divisibleBy;
  }
  EXPECT_EQ(day11::MODULO, modulo); // precalculated

  for (int i = 0; i < n; i++) {
    int monkey_id = {0};
    for (auto &m : monkeys) {
      while (!m.items.empty()) {
        counts[monkey_id] += 1;
        auto worry_level = m.items.front();
        m.items.erase(m.items.begin()); // removes the first element
        worry_level = m.operation(worry_level);
        if (is_part_1 ) worry_level = day11::Div( worry_level, 3);
        const int new_monkey = day11::IsDiv( worry_level, m.divisibleBy ) ? m.monkeyTrue : m.monkeyFalse;
        monkeys[new_monkey].items.push_back(worry_level);
      }
      monkey_id++;
    }
    // round is over
    EXPECT_TRUE(true);
  }
  std::ranges::sort(counts,
                    std::ranges::greater()); // first element will be greatest

  constexpr auto ANSWER1 = day11::IS_TEST ? 10605 : 118674;
  constexpr auto ANSWER2 = day11::IS_TEST ? 2713310158 : 32333418600;   

  EXPECT_EQ(counts[0] * counts[1], is_part_1 ? ANSWER1 : ANSWER2 );
}