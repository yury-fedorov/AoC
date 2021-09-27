#include "common.h"

using namespace std;

namespace day12 {
    int answer1() {return 0;}
    int answer2() {return 1;}
}

TEST_CASE( "Day12", "[12]" ) {
    using namespace day12;

    ifstream f("../data/day12.txt");
    string line;
    getline(f, line);

    SECTION( "12-1" ) {
        REQUIRE( answer1() == -1 );
    }

    SECTION( "12-2" ) {
        REQUIRE( answer2() == -2 );
    }
}
