#include "absl/strings/numbers.h"
#include "common.h"
#include "absl/container/flat_hash_map.h"

namespace day07 {
  enum class NodeType { FILE, DIR };
  struct Node;
  using Filesystem = absl::flat_hash_map<std::string,Node>;
  struct Node {
    std::string full_name_;
    NodeType type_;
    size_t size_; // for dir is 0
    std::unique_ptr<Filesystem> dir_; // for file is empty;
  };
  constexpr char root_char = '/';
  constexpr std::string_view root = "/"; // ? char to str
  std::string Compose( std::string_view path, std::string_view name ) {
    std::string result = path;
    if ( !result.ends_with( root_char ) ) result += root_char;
    return absl::StrCat( result, name );
  }
}

TEST(AoC22, Day07) {
  const auto data = ReadData("07");
  constexpr std::string_view level_up = ".."; 
  std::string path {day07::root};
  day07::Filesystem fs;
  day07::Filesystem & cur_dir = fs;
  for (const std::string &line : data) {
    constexpr std::string_view cd_ = "$ cd ";
    constexpr std::string_view ls = "$ ls";
    constexpr std::string_view dir_ = "dir ";
    if ( line.starts_with( cd_ ) ) {
      const auto dir = line.substr( cd_.length());
      if ( dir == day07::root ) path = day07::root;
      else if ( dir == level_up ) {
        path = path.substr( 0, path.find_last_of( day07::root_char ) );
      } 
      else {
        path = day07::Compose( path, dir );
      } 
    } else if ( line == ls ) {
      // ?    
    } else if ( line.starts_with(dir_) ) {
      // dir bhmndjpq
      const auto dir_name = line.substr( dir_.length() );
      const auto dir = std::make_unique<day07::Filesystem>();
      cur_node[dir_name] = { day07::Compose(path, dir_name), day07::NodeType::DIR, 0, dir };
    } else {
      // 97867 mqbz.fcp
      const auto p = absl::StrSplit(line, " ");
      const auto & file_name = p[1];
      if ( size_t size {0}; absl::SimpleAtoi(p[0], &size) ) {
        cur_node[file_name] = { day07::Compose(path, file_name), day07::NodeType::FILE, size, nullptr };
      } else FAIL();
    }
  }
  /*
  const auto sum_highest_n = [&sums](size_t n) {
    return std::accumulate(sums.rbegin(), sums.rbegin() + n, 0);
  };
  EXPECT_EQ(sum_highest_n(1), 70698);
  EXPECT_EQ(sum_highest_n(3), 206643);
  */
}