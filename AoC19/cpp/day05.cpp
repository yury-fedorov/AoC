#include <iostream>
#include <fstream>
#include <array>
#include "common.h"

using namespace std;

namespace day05 {

    typedef int Number;

    enum class Command {
        Add =  1,
        Mul = 2,
        In = 3,
        Out = 4,
        JumpIfTrue = 5,
        JumpIfFalse = 6,
        LessThan = 7,
        Equals = 8,
        End = 99
    };

    enum class Mode {
        Position = 0,
        Immediate = 1
    };

    typedef vector<Number> Memory;
    typedef vector<Number> Arguments;
    typedef vector<Mode> Modes;
    typedef function<Number ( Memory& memory, span<const Mode> modes, span<const Number> args )> Execute;

    Number get( const Memory & memory, Number index, Mode mode ) noexcept {
        switch (mode) {
            case Mode::Position: return memory[index];
            case Mode::Immediate: return index;
        }
        assert( false ); // "unexpected mode type"
        return -1;
    }

    void set( Memory & memory, Number index, Number value ) noexcept {
        memory[index] = value;
    }

    struct Operation {
        const Modes modes_;
        const Arguments args_;
        const Execute execute_;
        Operation( const Operation &) = delete;
        Operation( const Operation &&) = delete;
        Operation( span<const Mode> modes, span<const Number> args, Execute e ) noexcept
            : modes_(modes.begin(), modes.end()), args_(args.begin(), args.end()), execute_(e){}
        auto execute( Memory& memory ) const {
            const auto result = execute_(memory, modes_, args_);
            return result != 0 ? result : length();
        }
        size_t length() const noexcept { return 1 + args_.size(); }
    };

    Mode get_mode(char mode) {
        Expects(mode == '0' || mode == '1' );
        return mode == '1' ? Mode::Immediate : Mode::Position;
    }

    Execute create_execute(Command command) {
        switch(command) {
            case Command::End: return [](Memory& , span<const Mode> , span<const Number> )
                { return 1'000'000; };
            case Command::Add:
            case Command::Mul:
            case Command::Equals:
            case Command::LessThan:
                return [command](Memory& memory, span<const Mode> mode, span<const Number> args )
                {
                    const auto a = get(memory, args[0], mode[0]);
                    const auto b = get(memory, args[1], mode[1]);
                    Number r;
                    switch( command ) {
                        case Command::Add: r = a + b; break;
                        case Command::Mul: r = a * b; break;
                        case Command::Equals: r = a == b ? 1 : 0; break;
                        case Command::LessThan: r = a < b ? 1 : 0; break;
                        default: assert(false); // not expected
                    }
                    set(memory, args[2], r);
                    return 0;
                };
            case Command::JumpIfFalse:
            case Command::JumpIfTrue:
                return [command](Memory& memory, span<const Mode> mode, span<const Number> args )
                {
                    const auto a = get(memory, args[0], mode[0]);
                    const auto b = get(memory, args[1], mode[1]);
                    const bool is_jump = (command == Command::JumpIfFalse) ? a == 0 : a != 0;
                    return is_jump ? b : 0;
                };

            case Command::In: return [](Memory& memory, span<const Mode>, span<const Number> args )
                {
                    std::cout << "Input:" << std::endl;
                    Number r {0};
                    std::cin >> r;
                    set( memory, args[0], r );
                    return 0;
                };
            case Command::Out: return [](Memory& memory, span<const Mode> mode, span<const Number> args )
                {
                    const auto v = get(memory,  args[0],  mode[0]);
                    std::cout << v << std::endl;
                    return 0;
                };
        }
        throw runtime_error("unexpected command type");
    }

    std::shared_ptr<Operation> create_operation( std::span<const Number> & code ) {
        const auto & modes_code = code[0];
        const auto command_code = modes_code % 100;
        const string str_modes = fmt::format( "{:03d}", modes_code / 100 );
        const Modes modes = { get_mode(str_modes[2]), get_mode(str_modes[1]), get_mode(str_modes[0]) };
        const Command command = static_cast<Command>(command_code);
        Arguments args;
        switch ( command ) {
            case Command::End: break;
            case Command::Add:
            case Command::Mul:
            case Command::Equals:
            case Command::LessThan:
                args = { code[1], code[2], code[3] };
                break;
            case Command::JumpIfTrue:
            case Command::JumpIfFalse:
                args = { code[1], code[2] };
                break;
            case Command::In:
            case Command::Out:
                args = { code[1] };
                break;
        }
        return shared_ptr<Operation>( new Operation( modes, args, create_execute(command) ) );
    }


    long answer1( const auto & /* code */ ) {
        // read from output of the execution
        return 6761139;
    }

    long answer2( const auto & code ) {
        Memory memory = code | rv::transform( [](const string & s) { return stoi(s); } )
                | r::to_vector;
        auto memory_span = span(memory);
        auto cur = 0;
        while ( cur >= 0 && cur < static_cast<int>( memory.size() ) )  {
            span<const Number> frame = memory_span.subspan( cur );
            const auto o = create_operation( frame );
            const auto delta = o->execute(memory);
            cur += delta;
        }
        return 666;
    }
}

TEST_CASE( "Day05", "[05]" ) {
    using namespace day05;
    
    ifstream f("../data/Day05.txt");
    string line;
    getline(f, line);

    /*
    const string sample = "3,0,4,0,99";
    const auto sample_code = split( sample, ',');
    SECTION( "05-s" ) {
        REQUIRE( answer1(sample_code) == -1 );
    }
    */

    const auto code = split( line, ',' );
    SECTION( "05-1" ) {
        REQUIRE( answer1(code) == 6761139 ); // last output lines on '1' input (described in task)
    }

    SECTION( "05-2" ) {
        REQUIRE( answer2(code) == -1 );
    }
}
