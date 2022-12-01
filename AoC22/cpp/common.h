#pragma once

#include <algorithm>
#include <fstream>
#include <iostream>
#include <map>
#include <numeric>
#include <ranges>
#include <span>
#include <string>
#include <string_view>
#include <vector>

#include "absl/strings/str_cat.h"
#include "absl/strings/str_split.h"
#include "gtest/gtest.h"

// to hide the external dependencies
// #include "range/v3/all.hpp" // CLang 12 does not support C++20 ranges
// namespace r = std::ranges; // standard is std::ranges
// namespace rv = std::ranges::views;
// namespace s = std; // namespace for span (defined in 3 different places:
// C++20, ranges v3, GSL)

inline bool is_fast_only() noexcept { return true; } // to filter out slow tests

inline std::vector<std::string> ReadData(std::string_view file) noexcept {
  const std::string filepath = absl::StrCat("../data/", file, ".txt");
  std::ifstream f(filepath);
  std::string content;
  std::getline(f, content, '\0'); // read all file
  return absl::StrSplit(content, '\n');
}