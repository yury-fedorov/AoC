#define once
#include <queue>
#include <vector>

namespace intcode_computer {
    typedef int Number;
    typedef std::vector<Number> Memory;
    typedef std::queue<Number> Queue;
    Memory load( const std::string & code );
    bool run( Memory memory, Queue& in, Queue& out ); // if halted - true
}

