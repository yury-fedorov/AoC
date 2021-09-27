#include "common.h"

using namespace std;

namespace day14 {
    int answer1() {return 0;}
    int answer2() {return 1;}
}

TEST_CASE( "Day14", "[14]" ) {
    using namespace day14;

    ifstream f("../data/day14.txt");
    string line;
    getline(f, line);

    SECTION( "14-1" ) {
        REQUIRE( answer1() == -1 );
    }

    SECTION( "14-2" ) {
        REQUIRE( answer2() == -2 );
    }
}
