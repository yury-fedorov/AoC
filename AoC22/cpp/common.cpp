#include "common.h"
#include <fstream>

[[nodiscard]] std::vector<std::string>
ReadData(std::string_view file) noexcept {
  const std::string filepath = absl::StrCat("../data/", file, ".txt");
  std::ifstream f(filepath);
  std::string content;
  std::getline(f, content, '\0'); // read all file
  std::vector<std::string> result = absl::StrSplit(content, '\n');
  // if the last line is empty, we delete it
  auto last = result.end() - 1;
  if (last->empty())
    result.erase(last);
  return result;
}