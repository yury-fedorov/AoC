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
// #include <format>

#include "gtest/gtest.h"

// to hide the external dependencies
// #include "fmt/format.h" // CLang does not support C++20 format library
// #include "gsl/gsl_assert"
// #include "gsl/span"
// #include "range/v3/all.hpp" // CLang 12 does not support C++20 ranges
// namespace r = std::ranges; // standard is std::ranges
// namespace rv = std::ranges::views;
// namespace s = std; // namespace for span (defined in 3 different places:
// C++20, ranges v3, GSL)

inline bool is_fast_only() noexcept { return true; } // to filter out slow tests

inline std::vector<std::string> ReadData(std::string_view file) noexcept {
  // could be optimized with absl
  std::string filename("../data/");
  filename += file;
  filename += ".txt";

  // const auto filename = std::format( "../data/{}.txt", file ); // Clang does
  // not support
  std::ifstream f(filename);
  std::string line;
  std::vector<std::string> list;
  while (!f.eof()) {
    std::getline(f, line);
    list.emplace_back(line);
  }
  return list;
}
