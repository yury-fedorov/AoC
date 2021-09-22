#pragma once

#include <algorithm>
#include <span>
#include <vector>
#include <catch2/catch.hpp>

// to hide the external dependencies
#include "fmt/format.h" // CLang does not support C++20 format library
#include "gsl/gsl_assert"
#include "range/v3/all.hpp" // CLang 12 does not support C++20 ranges
namespace r = ranges; // standard is std::ranges
namespace rv = ranges::views;

std::vector<std::string> split(const std::string& s, char delimiter);
