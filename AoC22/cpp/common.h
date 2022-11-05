#pragma once

#include <algorithm>
#include <fstream>
#include <map>
#include <span>
#include <string_view>
#include <vector>
#include <ranges>
#include "gtest/gtest.h"

inline bool is_fast_only() noexcept { return true; } // to filter out slow tests

