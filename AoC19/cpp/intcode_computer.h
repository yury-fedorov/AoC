#define once
#include <queue>
#include <vector>

namespace intcode_computer {
    using Number = long long;
    using Memory = std::vector<Number>;
    using Queue = std::queue<Number>;
    Memory load( const std::string & code );
    bool run( Memory memory, Queue& in, Queue& out ); // if halted - true
}

