#include <iostream>
#include <fstream>
#include <queue>
#include "common.h"
#include "intcode_computer.h"

using namespace std;
using namespace intcode_computer;

namespace day05 {
    Number solution( const auto & code, Number input ) {
        Queue in;
        in.push( input );
        Queue out;
        run(code, in,  out);
        return out.back();
    }
    Number answer1( const auto & code ) { return solution(code, 1); }
    Number answer2( const auto & code ) { return solution(code, 5); }
}

TEST_CASE( "Day05", "[05]" ) {
    using namespace day05;
    
    ifstream f("../data/Day05.txt");
    string line;
    getline(f, line);

    const string sample = "3,0,4,0,99";
    const auto sample_code = load(sample);
    SECTION( "05-s" ) {
        assert( solution(sample_code, 1 ) == 1 );
    }

    const auto code = load(line);
    SECTION( "05-1" ) {
        REQUIRE( answer1(code) == 6761139 );
    }

    SECTION( "05-2" ) {
        REQUIRE( answer2(code) == 9217546 );
    }
}