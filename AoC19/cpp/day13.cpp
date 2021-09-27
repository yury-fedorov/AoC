#include "common.h"

using namespace std;

namespace day13 {
    int answer1() {return 0;}
    int answer2() {return 1;}
}

TEST_CASE( "Day13", "[13]" ) {
    using namespace day13;

    ifstream f("../data/day13.txt");
    string line;
    getline(f, line);

    SECTION( "13-1" ) {
        REQUIRE( answer1() == -1 );
    }

    SECTION( "13-2" ) {
        REQUIRE( answer2() == -2 );
    }
}
