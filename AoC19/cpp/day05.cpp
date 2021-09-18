#include <iostream>
#include <fstream>
#include <span>
#include "common.h"

using namespace std;

namespace day05 {

    typedef int Number;
    typedef std::vector<Number> Code;

    enum class Command {
        Add =  1,
        Mul = 2,
        In = 3,
        Out = 4,
        End = 99
    };

    struct Operation {
        const Command command_;
        Operation( Command command ) : command_ (command ) {}
    };

    struct End : Operation {
        End() : Operation( Command::End ) {}
    };

    struct MathOperation : Operation {
        const Number arg0_;
        const Number arg1_;
        const Number result_;
        MathOperation( Command command, Number a, Number b, Number c )
            : Operation( command ), arg0_(a), arg1_(b), result_(c) {}
    };

    struct InputOutputOperation : Operation {
         const Number address_;
        InputOutputOperation(Command command, Number a) : Operation(command), address_(a) {}
    };

    Number add( Number a, Number b ) { return a + b; }
    Number mul( Number a, Number b ) { return a * b; }
    Number in() { std::cout << "Input:" << std::endl; Number r {0}; std::cin >> r; return r; }
    void out(Number a) { std::cout << a << std::endl; }

    std::shared_ptr<Operation> exec_op( const std::span<Number> & code ) {
        Operation * result = nullptr;
        const Command command = static_cast<Command>(code[0]);
        switch ( command ) {
            case Command::End: result = new End(); break;
            case Command::Add:
            case Command::Mul:
                result = new MathOperation( command, code[1], code[2], code[3] );
                break;
            case Command::In:
            case Command::Out:
                result = new InputOutputOperation( command, code[1] );
                break;
        }
        return std::shared_ptr<Operation>( result );
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
