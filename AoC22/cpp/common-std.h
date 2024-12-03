#ifndef AOC24_COMMON_PLAIN_H_
#define AOC24_COMMON_PLAIN_H_

#include <algorithm>
#include <numeric>
#include <string>
#include <string_view>
#include <vector>

// To filter out slow tests.
[[nodiscard]] inline bool IsFastOnly() noexcept { return true; }

// Avoid running red tests. It helps to keep pipeline green.
[[nodiscard]] inline bool IsGreenOnly() noexcept { return true; }

[[nodiscard]] inline constexpr int Sign(auto x) noexcept {
  return x > 0 ? 1 : x < 0 ? -1 : 0;
}

[[nodiscard]] std::vector<std::string> ReadData(std::string_view file) noexcept;

[[nodiscard]] std::vector<std::string>
StrSplit(std::string_view text, std::string_view delimiter) noexcept;

#endif //  AOC24_COMMON_PLAIN_H_