// #include "absl/strings/str_split.h"
#include "common-std.h"
#include <fstream>

[[nodiscard]] std::vector<std::string>
StrSplit(std::string_view text, std::string_view delimiter) noexcept {
  size_t pos;
  // = text.find(delimiter);
  // if (pos == std::string::npos) return { std::string(text) };

  std::vector<std::string> tokens;
  const size_t delim_len = delimiter.length();
  while ((pos = text.find(delimiter)) != std::string::npos) {
    tokens.push_back(std::string(text.substr(0, pos)));
    text = text.substr(pos + delim_len);
  }
  tokens.push_back(std::string(text));
  return tokens;
  // return absl::StrSplit(text, delimiter);
}

[[nodiscard]] std::vector<std::string>
ReadData(std::string_view file) noexcept {
  const std::string filepath =
      std::string("../data/") + std::string(file) + std::string(".txt");
  std::ifstream f(filepath);
  std::string content;
  std::getline(f, content, '\0'); // read all file
  std::vector<std::string> result = StrSplit(content, "\n");
  // if the last line is empty, we delete it
  auto last = result.end() - 1;
  if (last->empty())
    result.erase(last);
  return result;
}
