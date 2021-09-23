#include "common.h"
#include "intcode_computer.h"

using namespace std;
using namespace intcode_computer;

namespace day07 {
    int answer1( const auto & ) { return 0; }
    int answer2( const auto & ) { return 0; }
}

TEST_CASE( "Day07", "[07]" ) {
    using namespace day07;

    ifstream f("../data/day07.txt");
    string line;
    getline(f, line);
    const Memory data = load( line );

    SECTION( "07-1" ) {
        REQUIRE( answer1(data) == -1 );
    }

    SECTION( "07-2" ) {
        REQUIRE( answer2(data) == -2 );
    }
}