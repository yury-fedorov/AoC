#define once
#include "common.h"
#include <queue>

namespace intcode_computer {
    enum class Command {
        Add =  1,
        Mul = 2,
        In = 3,
        Out = 4,
        JumpIfTrue = 5,
        JumpIfFalse = 6,
        LessThan = 7,
        Equals = 8,
        AdjustRelativeBase = 9,
        End = 99
    };

    enum class Mode {
        Position = 0,
        Immediate = 1,
        Relative = 2
    };

    enum class JumpType {
        Next, // no jump
        Absolute
    };

    using Number = long long;
    using Memory = std::vector<Number>;
    using Queue = std::queue<Number>;

    using Arguments = std::vector<Number>;
    using Modes = std::vector<Mode>;
    using Jump = std::pair<JumpType,int>;
    using Execute = std::function<Jump ( Memory& memory, Queue& in, Queue& out,
        s::span<const Mode> modes, s::span<const Number> args,
        Number & relative_base )>;

    struct Operation {
        const Command command_;
        const Modes modes_;
        const Arguments args_;
        const Execute execute_;
        Operation( Command command, s::span<const Mode> modes, s::span<const Number> args, Execute e ) noexcept
                : command_(command), modes_(modes.begin(), modes.end()),
                  args_(args.begin(), args.end()), execute_(e){}
        Jump execute( Memory& memory, Queue& in, Queue& out, Number & relative_base ) const {
            return execute_(memory, in, out, modes_, args_, relative_base);
        }
        size_t length() const noexcept { return 1 + args_.size(); }
    };

    // using OnStep = function<  >
    Memory load( const std::string & code );
    bool run( Memory memory, Queue& in, Queue& out ); // if halted - true
}

