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

    struct Operation {};

    struct End : Operation {};

    struct MathOperation : Operation {
        Number result;
        Number result_index;
        Number position;
    };

    Number add( Number a, Number b ) { return a + b; }
    Number mul( Number a, Number b ) { return a * b; }
    Number in() { std::cout << "Input:" << std::endl; Number r {0}; std::cin >> r; return r; }
    void out(Number a) { std::cout << a << std::endl; }

    std::shared_ptr<Operation> exec_op( const std::span<Number> & ) {
        return std::shared_ptr<Operation>( new End() );
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

    const auto code = split( line, ',' );

    SECTION( "05-1" ) {
        REQUIRE( answer1(code) == 744475 );
    }

    SECTION( "05-2" ) {
        REQUIRE( answer2(line) == 70276940 );
    }
}
