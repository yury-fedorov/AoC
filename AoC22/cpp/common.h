#pragma once

#include <algorithm>
#include <fstream>
#include <numeric>
#include <ranges>
#include <string>
#include <string_view>
#include <vector>

#include "absl/strings/str_cat.h"
#include "absl/strings/str_split.h"
#include "gtest/gtest.h"

inline bool is_fast_only() noexcept { return true; } // to filter out slow tests

inline std::vector<std::string> ReadData(std::string_view file) noexcept {
  const std::string filepath = absl::StrCat("../data/", file, ".txt");
  std::ifstream f(filepath);
  std::string content;
  std::getline(f, content, '\0'); // read all file
  return absl::StrSplit(content, '\n');
}