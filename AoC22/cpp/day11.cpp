#include "absl/status/statusor.h"
#include "absl/strings/numbers.h"
#include "common.h"
#include <re2/re2.h>

namespace day11 {
using Operation = std::function<int(int)>;
struct Monkey {
  std::vector<int> items;
  Operation operation;
  int divisibleBy;
  int monkeyTrue;
  int monkeyFalse;
  Monkey(std::vector<int> items_, Operation operation_, int divisibleBy_,
         int monkeyTrue_, int monkeyFalse_)
      : items(items_), operation(operation_), divisibleBy(divisibleBy_),
        monkeyTrue(monkeyTrue_), monkeyFalse(monkeyFalse_) {}
  Monkey(const Monkey &other) = default;
};
// constexpr
Operation OpAdd(int arg) {
  return [arg](int old) -> int { return old + arg; };
};
Operation OpMul(int arg) {
  return [arg](int old) -> int { return old * arg; };
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
    return [](int old) -> int { return old * old; };
  }
  return absl::InvalidArgumentError(
      absl::StrCat("Unexpected pattern: ", operation));
}

// TODO - ranges / views?
std::vector<int> ToInt(const std::vector<std::string> &v) {
  std::vector<int> result;
  for (const auto &l : v) {
    if (int value{0}; absl::SimpleAtoi(l, &value)) {
      result.push_back(value);
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

  EXPECT_EQ(day11::OpCreate("new = old * 17").value()(1), 17);
  EXPECT_EQ(day11::OpCreate("new = old + 17").value()(1), 18);
  EXPECT_EQ(day11::OpCreate("new = old * old").value()(2), 4);

  const auto data = ReadData // ("11-sample");
      ("11");
  std::vector<day11::Monkey> monkeys;

  for (int i = 0; i < data.size(); i++) {
    const auto &line = data[i];
    if (!line.starts_with("Monkey"))
      continue;
    // found a monkey description
    const std::vector<std::string> parts_items =
        absl::StrSplit(data[i + 1], ": ");
    const std::vector<int> items =
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
  std::vector<int> counts(monkeys.size(), 0); // all values set to 0

  for (int i = 0; i < 20; i++) {
    int monkey_id = {0};
    for (auto &m : monkeys) {
      while (!m.items.empty()) {
        counts[monkey_id] += 1;
        int worry_level = m.items.front();
        m.items.erase(m.items.begin()); // removes the first element
        worry_level = m.operation(worry_level) / 3;
        const int new_monkey =
            worry_level % m.divisibleBy == 0 ? m.monkeyTrue : m.monkeyFalse;
        monkeys[new_monkey].items.push_back(worry_level);
      }
      monkey_id++;
    }
    // round is over
    EXPECT_TRUE(true);
  }
  std::ranges::sort(counts,
                    std::ranges::greater()); // first element will be greatest

  EXPECT_EQ(counts[0] * counts[1], 118674); // 119358 is too high
  // EXPECT_EQ(sum_highest_n(3), 206643);
}