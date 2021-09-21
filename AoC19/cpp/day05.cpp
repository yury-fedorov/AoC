#include <iostream>
#include <fstream>
#include <array>
#include "absl/strings/str_format.h"
#include "gsl/gsl_assert"
#include "common.h"

using namespace std;

namespace day05 {

    typedef int Number;

    enum class Command {
        Add =  1,
        Mul = 2,
        In = 3,
        Out = 4,
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
            return length() + execute_(memory, modes_, args_);
        }
        size_t length() const noexcept { return 1 + args_.size(); }
    };

    Number add( Number a, Number b ) { return a + b; }
    Number mul( Number a, Number b ) { return a * b; }
    Number in() { std::cout << "Input:" << std::endl; Number r {0}; std::cin >> r; return r; }
    void out(Number a) { std::cout << a << std::endl; }

    Mode get_mode(char mode) {
        Expects(mode == '0' || mode == '1' );
        return mode == '1' ? Mode::Immediate : Mode::Position; }

    std::shared_ptr<Operation> create_operation( std::span<const Number> & code ) {
        const auto & modes_code = code[0];
        const auto command_code = modes_code % 100;
        const string str_modes = absl::StrFormat( "%03d", modes_code / 100 );
        const Modes modes = { get_mode(str_modes[0]), get_mode(str_modes[1]), get_mode(str_modes[2]) };
        const Command command = static_cast<Command>(command_code);
        Arguments args;
        Execute f = [](Memory& , span<const Mode> , span<const Number> ) { return 1'000'000; };
        switch ( command ) {
            case Command::End: break;
            case Command::Add:
            case Command::Mul:
                args = { code[1], code[2], code[3] };
                break;
            case Command::In:
            case Command::Out:
                args = { code[1] };
                break;
        }
        return shared_ptr<Operation>( new Operation( modes, args, f ) );
    }

    long long answer1( const auto &  ) {
        throw domain_error( "answer is not found" );
    }

    long long answer2( const string_view ) {
        throw domain_error( "answer is not found" );
    }
}

TEST_CASE( "Day05", "[05]" ) {
    using namespace day05;
    
    ifstream f("../data/Day05.txt");
    string line;
    getline(f, line);

    const string sample = "3,0,4,0,99";
    const auto sample_code = split( sample, ',');

    const auto code = split( line, ',' );

    SECTION( "05-1" ) {
        REQUIRE( answer1(code) == 744475 );
    }

    SECTION( "05-2" ) {
        REQUIRE( answer2(line) == 70276940 );
    }
}
