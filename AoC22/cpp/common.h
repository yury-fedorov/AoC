#pragma once

#include <algorithm>
#include <fstream>
#include <numeric>
// #include <ranges>
#include <range/v3/all.hpp> // get everything for external implementation
#include <string>
#include <string_view>
#include <vector>

#include "absl/strings/str_cat.h"
#include "absl/strings/str_split.h"
#include "gtest/gtest.h"

// mapping to hide external dependency on ranges
namespace r = ranges;
namespace rv = ranges::views;

[[nodiscard]] inline bool IsFastOnly() noexcept { return true; } // to filter out slow tests

inline std::vector<std::string> ReadData(std::string_view file) noexcept {
  const std::string filepath = absl::StrCat("../data/", file, ".txt");
  std::ifstream f(filepath);
  std::string content;
  std::getline(f, content, '\0'); // read all file
  return absl::StrSplit(content, '\n');
}