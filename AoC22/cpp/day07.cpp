// #include "absl/container/flat_hash_map.h"
#include "absl/strings/numbers.h"
#include "common.h"

namespace day07 {
enum class NodeType { FILE, DIR };
struct Node;
using NodePtr = std::unique_ptr<Node>;
// using FileSystem = absl::flat_hash_map<std::string, NodePtr>;
using FileSystem = std::vector<NodePtr>;
using DirPtr = std::unique_ptr<FileSystem>;
struct Node {
  NodeType type_;
  size_t size_;  // for dir is 0
  DirPtr dir_;   // nullptr for file
  Node *parent_; // we need it for cd .. // nullptr for root
  Node(NodeType type, size_t size, DirPtr dir, Node *parent)
      : type_(type), size_(size), dir_(std::move(dir)), parent_(parent) {}
  // Node( const Node & node ) : type_( node.type_ ), size_( node.size_ ), dir_(
  // node.dir_), parent_( node.parent_ ) {}
  // Node(Node&&) = default;
  bool IsFile() const { return type_ == NodeType::FILE; }
  bool IsDir() const { return type_ == NodeType::DIR; }
};
constexpr char root_char = '/';
constexpr std::string_view root = "/"; // ? char to str
std::string Compose(std::string_view path, std::string_view name) {
  std::string result(path);
  if (!result.ends_with(root_char))
    result += root_char;
  return absl::StrCat(result, name);
}
size_t Size(const Node *node) {
  if (node->IsFile())
    return node->size_;
  size_t sum{0};
  for (const auto &i : *node->dir_) {
    // const auto &sub_node = i.second;
    const auto &sub_node = i;
    sum += Size(sub_node.get());
  }
  return sum;
}
size_t Answer1(const Node *node) {
  constexpr auto max_dir_size = 100000;
  size_t answer{0};
  if (node->IsDir()) {
    const auto dir_size = Size(node);
    if (dir_size <= max_dir_size) {
      answer += dir_size;
    }
    for (const auto &i : *node->dir_) {
      // const auto &sub_node = i.second;
      const auto &sub_node = i;
      if (sub_node->IsDir()) {
        answer += Answer1(sub_node.get());
      }
    }
  }
  return answer;
}
NodePtr CreateDir(Node *parent) {
  return std::unique_ptr<Node>(new Node(
      day07::NodeType::DIR, static_cast<size_t>(0), DirPtr(), parent));
}
NodePtr CreateFile(Node *parent, size_t size) {
  return std::unique_ptr<Node>(
      new Node(day07::NodeType::FILE, size, DirPtr(), parent));
}

constexpr auto total_space = 70000000;
constexpr auto need_space = 30000000;
size_t Answer2(const Node *node, size_t need_to_free) {
  size_t answer{total_space};
  if (node->IsDir()) {
    const auto dir_size = Size(node);
    if (dir_size >= need_to_free) {
      answer = std::min(answer, dir_size);
    }
    for (const auto &i : *node->dir_) {
      // const auto &sub_node = i.second;
      const auto &sub_node = i;
      if (sub_node->IsDir()) {
        const auto alternative = Answer2(sub_node.get(), need_to_free);
        if (alternative >= need_to_free) {
          answer = std::min(answer, alternative);
        }
      }
    }
  }
  return answer;
}

} // namespace day07

TEST(AoC22, Day07) {
  const auto data = ReadData("07");
  constexpr std::string_view level_up = "..";
  std::string path{day07::root};
  auto root_node = day07::CreateDir(nullptr);
  day07::Node *cur_dir = root_node.get();
  for (const std::string &line : data) {
    constexpr std::string_view cd_ = "$ cd ";
    constexpr std::string_view ls = "$ ls";
    constexpr std::string_view dir_ = "dir ";
    if (line.starts_with(cd_)) {
      const auto dir = line.substr(cd_.length());
      if (dir == day07::root) {
        cur_dir = root_node.get();
      } else if (dir == level_up) {
        cur_dir = cur_dir->parent_;
      } else {
        auto new_dir = day07::CreateDir(cur_dir);
        auto new_dir_ptr = new_dir.get();
        cur_dir->dir_->emplace_back(std::move(new_dir));
        // change cur dir
        cur_dir = new_dir_ptr;
      }
    } else if (line == ls) {
      // ?
    } else if (line.starts_with(dir_)) {
      // dir bhmndjpq
      const auto sub_dir = line.substr(dir_.length());
      auto new_dir = day07::CreateDir(cur_dir);
      cur_dir->dir_->emplace_back(std::move(new_dir));
    } else if (!line.empty()) {
      // 97867 mqbz.fcp
      const std::vector<std::string_view> p = absl::StrSplit(line, " ");
      const auto &file_name = p[1];
      if (size_t size{0}; absl::SimpleAtoi(p[0], &size)) {
        auto new_file = day07::CreateFile(cur_dir, size);
        cur_dir->dir_->emplace_back(std::move(new_file));
      } else {
        EXPECT_EQ(absl::StrCat(line, " ", p[0]), "");
      }
    }
  }
  EXPECT_EQ(day07::Answer1(root_node.get()), 1084134);

  const auto occupied_space = day07::Size(root_node.get());
  const auto total_free_now = day07::total_space - occupied_space;
  const auto need_to_free = day07::need_space - total_free_now;
  EXPECT_EQ(day07::Answer2(root_node.get(), need_to_free), 6183184);
}