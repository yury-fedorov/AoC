#include "common.h"

using namespace std;

namespace day10 {

    using Map = vector<string>;

    auto answer1( const auto & map ) {
        return (int)map.size();
    }

    auto answer2( const auto & ) {
        return 0;
    }
}

TEST_CASE( "Day10", "[10]" ) {
    using namespace day10;

    Map map;
    ifstream f("../data/day10.txt");
    string line;
    while ( getline(f, line) ) {
        map.push_back(line);
    }

    SECTION( "10-1" ) {
        REQUIRE( answer1(map) == -1 );
    }

    SECTION( "10-2" ) {
        REQUIRE( answer2(map) == -2 );
    }
}
