#pragma once

#include <algorithm>
#include <numeric>
#include <fstream>
#include <map>
#include <set>
#include <span>
#include <string_view>
#include <vector>
#include <ranges>
#include <catch2/catch.hpp>

// to hide the external dependencies
// #include "fmt/format.h" // CLang does not support C++20 format library
// #include "gsl/gsl_assert"
// #include "gsl/span"
// #include "range/v3/all.hpp" // CLang 12 does not support C++20 ranges
namespace r = std::ranges; // standard is std::ranges
namespace rv = std::ranges::views;
namespace s = std; // namespace for span (defined in 3 different places: C++20, ranges v3, GSL)

inline bool is_fast_only() noexcept { return true; } // to filter out slow tests

std::vector<std::string> split(const std::string& s, char delimiter);
